/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "ScriptController.h"

#include "ContentSecurityPolicy.h"
#include "DocumentLoader.h"
#include "Frame.h"
#include "Document.h"
#include "FrameLoaderClient.h"
#include "Page.h"
#include "ScriptSourceCode.h"
#include "ScriptValue.h"
#include "Settings.h"

namespace WebCore {

bool ScriptController::canExecuteScripts(ReasonForCallingCanExecuteScripts reason)
{
    // FIXME: We should get this information from the document instead of the frame.
    if (m_frame->document()->isSandboxed(SandboxScripts))
        return false;

    Settings& settings = m_frame->settings();
    const bool allowed =settings.isJavaEnabled();//CMP_ERROR,no allowJavaScript; m_frame->loader().client().allowJavaScript( settings.isJavaEnabled());
    if (!allowed && reason == AboutToExecuteScript)
        m_frame->loader().client().didNotAllowScript();
    return allowed;
}

ScriptValue ScriptController::executeScript(const String& script, bool forceUserGesture)
{
    return executeScript(ScriptSourceCode(script, forceUserGesture ? KURL() : m_frame->document()->url()));
}

ScriptValue ScriptController::executeScript(const ScriptSourceCode& sourceCode)
{
    if (!canExecuteScripts(AboutToExecuteScript) || isPaused())
        return ScriptValue();

    bool wasInExecuteScript = m_inExecuteScript;
    m_inExecuteScript = true;

    ScriptValue result = evaluate(sourceCode);

    if (!wasInExecuteScript) {
        m_inExecuteScript = false;
        Document::updateStyleForAllDocuments();
    }

    return result;
}

bool ScriptController::executeIfJavaScriptURL(const KURL& url, ShouldReplaceDocumentIfJavaScriptURL shouldReplaceDocumentIfJavaScriptURL)
{
    if (!protocolIsJavaScript(url))
        return false;

    if (!m_frame->page()
        || !m_frame->page()->javaScriptURLsAreAllowed()
        //CMP_ERROR_UNCLEAR,func not match|| !m_frame->document()->contentSecurityPolicy()->allowJavaScriptURLs()
        || m_frame->inViewSourceMode())
        return true;

    // We need to hold onto the Frame here because executing script can
    // destroy the frame.
    RefPtr<Frame> protector(m_frame);

    const int javascriptSchemeLength = sizeof("javascript:") - 1;

    String decodedURL = decodeURLEscapeSequences(url.string());
    ScriptValue result = executeScript(decodedURL.substring(javascriptSchemeLength), false);

    // If executing script caused this frame to be removed from the page, we
    // don't want to try to replace its document!
    if (!m_frame->page())
        return true;

    String scriptResult;
#if USE(JSC)
    JSDOMWindowShell* shell = windowShell(mainThreadNormalWorld());
    JSC::ExecState* exec = shell->window()->globalExec();
    if (!result.getString(exec, scriptResult))
        return true;
#else
    if (!result.getString(scriptResult))
        return true;
#endif

    // FIXME: We should always replace the document, but doing so
    //        synchronously can cause crashes:
    //        http://bugs.webkit.org/show_bug.cgi?id=16782
    if (shouldReplaceDocumentIfJavaScriptURL == ReplaceDocumentIfJavaScriptURL) {
        // We're still in a frame, so there should be a DocumentLoader.
        ASSERT(m_frame->document()->loader());
        
        // DocumentWriter::replaceDocument can cause the DocumentLoader to get deref'ed and possible destroyed,
        // so protect it with a RefPtr.
        if (RefPtr<DocumentLoader> loader = m_frame->document()->loader())
            loader->writer().replaceDocument(scriptResult,m_frame->document());
    }
    return true;
}

} // namespace WebCore
