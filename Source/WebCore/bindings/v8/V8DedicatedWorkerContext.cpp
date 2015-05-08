/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#include "V8DedicatedWorkerContext.h"

#if ENABLE(WORKERS)

#include "EventListener.h"
#include "RuntimeEnabledFeatures.h"
#include "V8AbstractEventListener.h"
#include "V8Binding.h"
#include "V8BindingMacros.h"
#include "V8BindingState.h"
#include "V8DOMWrapper.h"
#include "V8IsolatedContext.h"
#include "V8Proxy.h"
#include "V8WorkerContext.h"

namespace WebCore {

WrapperTypeInfo V8DedicatedWorkerContext::info = { V8DedicatedWorkerContext::GetTemplate, V8DedicatedWorkerContext::derefObject, 0, &V8WorkerContext::info };

namespace DedicatedWorkerContextInternal {

template <typename T> void V8_USE(T) { }

static v8::Handle<v8::Value> onmessageAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.DedicatedWorkerContext.onmessage._get");
    DedicatedWorkerContext* imp = V8DedicatedWorkerContext::toNative(info.Holder());
    return imp->onmessage() ? v8::Handle<v8::Value>(static_cast<V8AbstractEventListener*>(imp->onmessage())->getListenerObject(imp->scriptExecutionContext())) : v8::Handle<v8::Value>(v8::Null());
}

static void onmessageAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.DedicatedWorkerContext.onmessage._set");
    DedicatedWorkerContext* imp = V8DedicatedWorkerContext::toNative(info.Holder());
    transferHiddenDependency(info.Holder(), imp->onmessage(), value, V8DedicatedWorkerContext::eventListenerCacheIndex);
    imp->setOnmessage(V8DOMWrapper::getEventListener(value, true, ListenerFindOrCreate));
    return;
}

} // namespace DedicatedWorkerContextInternal

static const BatchedAttribute DedicatedWorkerContextAttrs[] = {
    // Attribute 'onmessage' (Type: 'attribute' ExtAttr: '')
    {"onmessage", DedicatedWorkerContextInternal::onmessageAttrGetter, DedicatedWorkerContextInternal::onmessageAttrSetter, 0 /* no data */, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), 0 /* on instance */},
};
static const BatchedCallback DedicatedWorkerContextCallbacks[] = {
    {"postMessage", V8DedicatedWorkerContext::postMessageCallback},
};
static v8::Persistent<v8::FunctionTemplate> ConfigureV8DedicatedWorkerContextTemplate(v8::Persistent<v8::FunctionTemplate> desc)
{
    v8::Local<v8::Signature> defaultSignature = configureTemplate(desc, "DedicatedWorkerContext", V8WorkerContext::GetTemplate(), V8DedicatedWorkerContext::internalFieldCount,
        DedicatedWorkerContextAttrs, WTF_ARRAY_LENGTH(DedicatedWorkerContextAttrs),
        DedicatedWorkerContextCallbacks, WTF_ARRAY_LENGTH(DedicatedWorkerContextCallbacks));
    v8::Local<v8::ObjectTemplate> instance = desc->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> proto = desc->PrototypeTemplate();
    

    // Custom toString template
    desc->Set(getToStringName(), getToStringTemplate());
    return desc;
}

v8::Persistent<v8::FunctionTemplate> V8DedicatedWorkerContext::GetRawTemplate()
{
    static v8::Persistent<v8::FunctionTemplate> V8DedicatedWorkerContextRawCache = createRawTemplate();
    return V8DedicatedWorkerContextRawCache;
}

v8::Persistent<v8::FunctionTemplate> V8DedicatedWorkerContext::GetTemplate()
{
    static v8::Persistent<v8::FunctionTemplate> V8DedicatedWorkerContextCache = ConfigureV8DedicatedWorkerContextTemplate(GetRawTemplate());
    return V8DedicatedWorkerContextCache;
}

bool V8DedicatedWorkerContext::HasInstance(v8::Handle<v8::Value> value)
{
    return GetRawTemplate()->HasInstance(value);
}


v8::Handle<v8::Object> V8DedicatedWorkerContext::wrapSlow(DedicatedWorkerContext* impl)
{
    v8::Handle<v8::Object> wrapper;
    V8Proxy* proxy = 0;
    wrapper = V8DOMWrapper::instantiateV8Object(proxy, &info, impl);
    if (wrapper.IsEmpty())
        return wrapper;

    impl->ref();
    v8::Persistent<v8::Object> wrapperHandle = v8::Persistent<v8::Object>::New(wrapper);
    getDOMObjectMap().set(impl, wrapperHandle);
    return wrapper;
}

void V8DedicatedWorkerContext::derefObject(void* object)
{
    static_cast<DedicatedWorkerContext*>(object)->deref();
}

} // namespace WebCore

#endif // ENABLE(WORKERS)
