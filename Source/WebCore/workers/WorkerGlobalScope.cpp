/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2009, 2011 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "config.h"
#include "WorkerGlobalScope.h"

#include "ActiveDOMObject.h"
#include "ContentSecurityPolicy.h"
#include "DOMTimer.h"
#include "DOMURL.h"
#include "DOMWindow.h"
#include "ErrorEvent.h"
#include "Event.h"
#include "EventException.h"
#include "ExceptionCode.h"
//#include "InspectorConsoleInstrumentation.h"
#include "MessagePort.h"
#include "ScheduledAction.h"
#include "ScriptSourceCode.h"
#include "SecurityOrigin.h"
#include "URL.h"
#include "WorkerInspectorController.h"
#include "WorkerLocation.h"
#include "WorkerNavigator.h"
#include "WorkerObjectProxy.h"
#include "WorkerScriptLoader.h"
#include "WorkerThread.h"
#include "WorkerThreadableLoader.h"
#include "XMLHttpRequestException.h"
#include <bindings/ScriptValue.h>
#include <inspector/ScriptCallStack.h>
#include <wtf/RefPtr.h>

#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
#include "NotificationCenter.h"
#endif

//using namespace Inspector;

namespace WebCore {

WorkerGlobalScope::WorkerGlobalScope(const URL& url, const String& userAgent, std::unique_ptr<GroupSettings> settings, WorkerThread& thread, PassRefPtr<SecurityOrigin> topOrigin)
    : m_url(url)
    , m_userAgent(userAgent)
    , m_groupSettings(WTF::move(settings))
//    , m_script(std::make_unique<WorkerScriptController>(this)) //CMP_ERROR_UNCLEAR
    , m_thread(thread)
#if ENABLE(INSPECTOR)
    , m_workerInspectorController(std::make_unique<WorkerInspectorController>(*this))
#endif
    , m_closing(false)
    , m_eventQueue(*this)
    , m_topOrigin(topOrigin)
{
    setSecurityOrigin(SecurityOrigin::create(url));
}

WorkerGlobalScope::~WorkerGlobalScope()
{
    ASSERT(currentThread() == thread().threadID());

    // Make sure we have no observers.
    notifyObserversOfStop();

    // Notify proxy that we are going away. This can free the WorkerThread object, so do not access it after this.
    thread().workerReportingProxy().workerGlobalScopeDestroyed();
}

void WorkerGlobalScope::applyContentSecurityPolicyFromString(const String& policy, ContentSecurityPolicy::HeaderType contentSecurityPolicyType)
{
    setContentSecurityPolicy(std::make_unique<ContentSecurityPolicy>(this));
    contentSecurityPolicy()->didReceiveHeader(policy, contentSecurityPolicyType);
}

URL WorkerGlobalScope::completeURL(const String& url) const
{
    // Always return a null URL when passed a null string.
    // FIXME: Should we change the URL constructor to have this behavior?
    if (url.isNull())
        return URL();
    // Always use UTF-8 in Workers.
    return URL(m_url, url);
}

String WorkerGlobalScope::userAgent(const URL&) const
{
    return m_userAgent;
}

void WorkerGlobalScope::disableEval(const String& errorMessage)
{
    //m_script->disableEval(errorMessage); //CMP_ERROR
}

WorkerLocation* WorkerGlobalScope::location() const
{
    if (!m_location)
        m_location = WorkerLocation::create(m_url);
    return m_location.get();
}

void WorkerGlobalScope::close()
{
    if (m_closing)
        return;

    // Let current script run to completion but prevent future script evaluations.
    // After m_closing is set, all the tasks in the queue continue to be fetched but only
    // tasks with isCleanupTask()==true will be executed.
    m_closing = true;
    postTask({ ScriptExecutionContext::Task::CleanupTask, [] (ScriptExecutionContext& context) {
        ASSERT_WITH_SECURITY_IMPLICATION(context.isWorkerGlobalScope());
        WorkerGlobalScope* workerGlobalScope = toWorkerGlobalScope(&context);
        // Notify parent that this context is closed. Parent is responsible for calling WorkerThread::stop().
        workerGlobalScope->thread().workerReportingProxy().workerGlobalScopeClosed();
    } });
}

WorkerNavigator* WorkerGlobalScope::navigator() const
{
    if (!m_navigator)
        m_navigator = WorkerNavigator::create(m_userAgent);
    return m_navigator.get();
}

void WorkerGlobalScope::postTask(Task task)
{
    thread().runLoop().postTask(WTF::move(task));
}

int WorkerGlobalScope::setTimeout(std::unique_ptr<ScheduledAction> action, int timeout)
{
    return DOMTimer::install(scriptExecutionContext(), WTF::move(action), timeout, true);
}

void WorkerGlobalScope::clearTimeout(int timeoutId)
{
    DOMTimer::removeById(scriptExecutionContext(), timeoutId);
}

int WorkerGlobalScope::setInterval(std::unique_ptr<ScheduledAction> action, int timeout)
{
    return DOMTimer::install(scriptExecutionContext(), WTF::move(action), timeout, false);
}

void WorkerGlobalScope::clearInterval(int timeoutId)
{
    DOMTimer::removeById(scriptExecutionContext(), timeoutId);
}

void WorkerGlobalScope::importScripts(const Vector<String>& urls, ExceptionCode& ec)
{
    ASSERT(contentSecurityPolicy());
    ec = 0;
    Vector<String>::const_iterator urlsEnd = urls.end();
    Vector<URL> completedURLs;
    for (Vector<String>::const_iterator it = urls.begin(); it != urlsEnd; ++it) {
        const URL& url = scriptExecutionContext()->completeURL(*it);
        if (!url.isValid()) {
            ec = SYNTAX_ERR;
            return;
        }
        completedURLs.append(url);
    }
    Vector<URL>::const_iterator end = completedURLs.end();

    for (Vector<URL>::const_iterator it = completedURLs.begin(); it != end; ++it) {
        RefPtr<WorkerScriptLoader> scriptLoader(WorkerScriptLoader::create());
        scriptLoader->loadSynchronously(scriptExecutionContext(), *it, AllowCrossOriginRequests);

        // If the fetching attempt failed, throw a NETWORK_ERR exception and abort all these steps.
        if (scriptLoader->failed()) {
            ec = XMLHttpRequestException::NETWORK_ERR;
            return;
        }

        //InspectorInstrumentation::scriptImported(scriptExecutionContext(), scriptLoader->identifier(), scriptLoader->script());

        Deprecated::ScriptValue exception;
        m_script->evaluate(ScriptSourceCode(scriptLoader->script(), scriptLoader->responseURL()), &exception);
        if (!exception.hasNoValue()) {
            m_script->setException(exception);
            return;
        }
    }
}

EventTarget* WorkerGlobalScope::errorEventTarget()
{
    return this;
}

void WorkerGlobalScope::logExceptionToConsole(const String& errorMessage, const String& sourceURL, int lineNumber, int columnNumber, PassRefPtr<ScriptCallStack>)
{
    thread().workerReportingProxy().postExceptionToWorkerObject(errorMessage, lineNumber, columnNumber, sourceURL);
}

void WorkerGlobalScope::addConsoleMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask(source, level, message));
        return;
    }

    thread().workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, 0, 0, String());
    addMessageToWorkerConsole(source, level, message, String(), 0, 0, 0, 0, requestIdentifier);
}

void WorkerGlobalScope::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack, ScriptState* state, unsigned long requestIdentifier)
{
    if (!isContextThread()) {
        postTask(AddConsoleMessageTask(source, level, message));
        return;
    }

    thread().workerReportingProxy().postConsoleMessageToWorkerObject(source, level, message, lineNumber, columnNumber, sourceURL);
    addMessageToWorkerConsole(source, level, message, sourceURL, lineNumber, columnNumber, callStack, state, requestIdentifier);
}

void WorkerGlobalScope::addMessageToWorkerConsole(MessageSource source, MessageLevel level, const String& message, const String& sourceURL, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack,  ScriptState* state, unsigned long requestIdentifier)
{
#if 0
    ASSERT(isContextThread());
    if (callStack)
        InspectorInstrumentation::addMessageToConsole(this, source, MessageType::Log, level, message, callStack, requestIdentifier);
    else
        InspectorInstrumentation::addMessageToConsole(this, source, MessageType::Log, level, message, sourceURL, lineNumber, columnNumber, state, requestIdentifier);
#endif
}

bool WorkerGlobalScope::isContextThread() const
{
    return currentThread() == thread().threadID();
}

bool WorkerGlobalScope::isJSExecutionForbidden() const
{
    return m_script->isExecutionForbidden();
}

WorkerGlobalScope::Observer::Observer(WorkerGlobalScope* context)
    : m_context(context)
{
    ASSERT(m_context && m_context->isContextThread());
    m_context->registerObserver(this);
}

WorkerGlobalScope::Observer::~Observer()
{
    if (!m_context)
        return;
    ASSERT(m_context->isContextThread());
    m_context->unregisterObserver(this);
}

void WorkerGlobalScope::Observer::stopObserving()
{
    if (!m_context)
        return;
    ASSERT(m_context->isContextThread());
    m_context->unregisterObserver(this);
    m_context = 0;
}

void WorkerGlobalScope::registerObserver(Observer* observer)
{
    ASSERT(observer);
    m_workerObservers.add(observer);
}

void WorkerGlobalScope::unregisterObserver(Observer* observer)
{
    ASSERT(observer);
    m_workerObservers.remove(observer);
}

void WorkerGlobalScope::notifyObserversOfStop()
{
    HashSet<Observer*>::iterator iter = m_workerObservers.begin();
    while (iter != m_workerObservers.end()) {
        WorkerGlobalScope::Observer* observer = *iter;
        observer->stopObserving();
        observer->notifyStop();
        iter = m_workerObservers.begin();
    }
}

WorkerEventQueue& WorkerGlobalScope::eventQueue() const
{
    return m_eventQueue;
}

#if ENABLE(SUBTLE_CRYPTO)
bool WorkerGlobalScope::wrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&)
{
    return false;
}

bool WorkerGlobalScope::unwrapCryptoKey(const Vector<uint8_t>&, Vector<uint8_t>&)
{
    return false;
}
#endif // ENABLE(SUBTLE_CRYPTO)

} // namespace WebCore
