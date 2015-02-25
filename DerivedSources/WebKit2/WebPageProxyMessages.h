/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebPageProxyMessages_h
#define WebPageProxyMessages_h

#include "Arguments.h"
#include "MessageEncoder.h"
#include "SandboxExtension.h"
#include "ShareableBitmap.h"
#include "SharedMemory.h"
#include "StringReference.h"
#include "WebContextMenuItemData.h"
#include "WebHitTestResult.h"
#include "WebPopupItem.h"
#include <WebCore/FloatQuad.h>
#include <WebCore/FloatRect.h>
#include <WebCore/IntRect.h>
#include <WebCore/TextCheckerClient.h>
#include <utility>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace IPC {
    class Connection;
    class DataReference;
}

namespace WTF {
    class String;
}

namespace WebCore {
    class ContentFilter;
    class Cursor;
    class ProtectionSpace;
    struct GrammarDetail;
    struct WindowFeatures;
    class Color;
    class TextCheckingRequestData;
    class IntSize;
    class ResourceResponse;
    class DragData;
    struct ViewportAttributes;
    class ResourceRequest;
    class FloatRect;
    class FloatPoint;
    class IntRect;
    class IntPoint;
    struct Highlight;
    class ResourceError;
    class AuthenticationChallenge;
    struct FileChooserSettings;
    class CertificateInfo;
}

namespace WebKit {
    class QuickLookDocumentData;
    struct AssistedNodeInformation;
    struct EditingRange;
    struct InteractionInformationAtPosition;
    struct PlatformPopupMenuData;
    class ContextMenuContextData;
    struct EditorState;
    struct WebPageCreationParameters;
    struct DictionaryPopupInfo;
    class InjectedBundleUserMessageEncoder;
    struct NavigationActionData;
    struct AttributedString;
}

namespace Messages {
namespace WebPageProxy {

static inline IPC::StringReference messageReceiverName()
{
    return IPC::StringReference("WebPageProxy");
}

class CreateNewPage {
public:
    typedef std::tuple<uint64_t, WebCore::ResourceRequest, WebCore::WindowFeatures, WebKit::NavigationActionData> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CreateNewPage"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint64_t&, WebKit::WebPageCreationParameters&> Reply;
    CreateNewPage(uint64_t frameID, const WebCore::ResourceRequest& request, const WebCore::WindowFeatures& windowFeatures, const WebKit::NavigationActionData& navigationActionData)
        : m_arguments(frameID, request, windowFeatures, navigationActionData)
    {
    }

    const std::tuple<uint64_t, const WebCore::ResourceRequest&, const WebCore::WindowFeatures&, const WebKit::NavigationActionData&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::ResourceRequest&, const WebCore::WindowFeatures&, const WebKit::NavigationActionData&> m_arguments;
};

//lxx, 20150213
class IsJavascriptPopupWindowIntercepted {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("IsJavascriptPopupWindowIntercepted"); }
    static const bool isSync = true;

    typedef IPC::Arguments<> Reply;
    IsJavascriptPopupWindowIntercepted()  : m_arguments()
    {
    }

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class ShowPage {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowPage"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class ClosePage {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ClosePage"); }
    static const bool isSync = false;

    explicit ClosePage(bool stopResponsivenessTimer)
        : m_arguments(stopResponsivenessTimer)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class AddMessageToConsole {
public:
    typedef std::tuple<String, unsigned, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("AddMessageToConsole"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send();

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<> Reply;
    AddMessageToConsole(const String& message, const unsigned& lineNumber, const String& sourceID)
        : m_arguments(message, lineNumber, sourceID)
    {
    }

    const std::tuple<const String&, const unsigned&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const unsigned&, const String&> m_arguments;
};

class RunJavaScriptAlert {
public:
    typedef std::tuple<uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunJavaScriptAlert"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send();

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<> Reply;
    RunJavaScriptAlert(uint64_t frameID, const String& message)
        : m_arguments(frameID, message)
    {
    }

    const std::tuple<uint64_t, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&> m_arguments;
};

class RunJavaScriptConfirm {
public:
    typedef std::tuple<uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunJavaScriptConfirm"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send(bool result);

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<bool&> Reply;
    RunJavaScriptConfirm(uint64_t frameID, const String& message)
        : m_arguments(frameID, message)
    {
    }

    const std::tuple<uint64_t, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&> m_arguments;
};

class RunJavaScriptPrompt {
public:
    typedef std::tuple<uint64_t, String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunJavaScriptPrompt"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send(const String& result);

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<String&> Reply;
    RunJavaScriptPrompt(uint64_t frameID, const String& message, const String& defaultValue)
        : m_arguments(frameID, message, defaultValue)
    {
    }

    const std::tuple<uint64_t, const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&, const String&> m_arguments;
};

class ShouldInterruptJavaScript {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShouldInterruptJavaScript"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class MouseDidMoveOverElement {
public:
    typedef std::tuple<WebKit::WebHitTestResult::Data, uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("MouseDidMoveOverElement"); }
    static const bool isSync = false;

    MouseDidMoveOverElement(const WebKit::WebHitTestResult::Data& hitTestResultData, uint32_t modifiers, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(hitTestResultData, modifiers, userData)
    {
    }

    const std::tuple<const WebKit::WebHitTestResult::Data&, uint32_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::WebHitTestResult::Data&, uint32_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidBeginTrackingPotentialLongMousePress {
public:
    typedef std::tuple<WebCore::IntPoint, WebKit::WebHitTestResult::Data> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidBeginTrackingPotentialLongMousePress"); }
    static const bool isSync = false;

    DidBeginTrackingPotentialLongMousePress(const WebCore::IntPoint& mouseDownPosition, const WebKit::WebHitTestResult::Data& hitTestResultData, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(mouseDownPosition, hitTestResultData, userData)
    {
    }

    const std::tuple<const WebCore::IntPoint&, const WebKit::WebHitTestResult::Data&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, const WebKit::WebHitTestResult::Data&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidRecognizeLongMousePress {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidRecognizeLongMousePress"); }
    static const bool isSync = false;

    explicit DidRecognizeLongMousePress(const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(userData)
    {
    }

    const std::tuple<const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidCancelTrackingPotentialLongMousePress {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidCancelTrackingPotentialLongMousePress"); }
    static const bool isSync = false;

    explicit DidCancelTrackingPotentialLongMousePress(const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(userData)
    {
    }

    const std::tuple<const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

#if ENABLE(NETSCAPE_PLUGIN_API)
class UnavailablePluginButtonClicked {
public:
    typedef std::tuple<uint32_t, String, String, String, String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UnavailablePluginButtonClicked"); }
    static const bool isSync = false;

    UnavailablePluginButtonClicked(uint32_t pluginUnavailabilityReason, const String& mimeType, const String& pluginURLString, const String& pluginspageAttributeURLString, const String& frameURLString, const String& pageURLString)
        : m_arguments(pluginUnavailabilityReason, mimeType, pluginURLString, pluginspageAttributeURLString, frameURLString, pageURLString)
    {
    }

    const std::tuple<uint32_t, const String&, const String&, const String&, const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t, const String&, const String&, const String&, const String&, const String&> m_arguments;
};
#endif

#if ENABLE(WEBGL)
class WebGLPolicyForURL {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WebGLPolicyForURL"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint32_t&> Reply;
    explicit WebGLPolicyForURL(const String& url)
        : m_arguments(url)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if ENABLE(WEBGL)
class ResolveWebGLPolicyForURL {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ResolveWebGLPolicyForURL"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint32_t&> Reply;
    explicit ResolveWebGLPolicyForURL(const String& url)
        : m_arguments(url)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

class DidChangeViewportProperties {
public:
    typedef std::tuple<WebCore::ViewportAttributes> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangeViewportProperties"); }
    static const bool isSync = false;

    explicit DidChangeViewportProperties(const WebCore::ViewportAttributes& attributes)
        : m_arguments(attributes)
    {
    }

    const std::tuple<const WebCore::ViewportAttributes&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::ViewportAttributes&> m_arguments;
};

class DidReceiveEvent {
public:
    typedef std::tuple<uint32_t, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceiveEvent"); }
    static const bool isSync = false;

    DidReceiveEvent(uint32_t type, bool handled)
        : m_arguments(type, handled)
    {
    }

    const std::tuple<uint32_t, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t, bool> m_arguments;
};

class StopResponsivenessTimer {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StopResponsivenessTimer"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

#if !PLATFORM(IOS)
class SetCursor {
public:
    typedef std::tuple<WebCore::Cursor> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetCursor"); }
    static const bool isSync = false;

    explicit SetCursor(const WebCore::Cursor& cursor)
        : m_arguments(cursor)
    {
    }

    const std::tuple<const WebCore::Cursor&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::Cursor&> m_arguments;
};
#endif

#if !PLATFORM(IOS)
class SetCursorHiddenUntilMouseMoves {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetCursorHiddenUntilMouseMoves"); }
    static const bool isSync = false;

    explicit SetCursorHiddenUntilMouseMoves(bool hiddenUntilMouseMoves)
        : m_arguments(hiddenUntilMouseMoves)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};
#endif

class SetStatusText {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetStatusText"); }
    static const bool isSync = false;

    explicit SetStatusText(const String& statusText)
        : m_arguments(statusText)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class SetToolTip {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetToolTip"); }
    static const bool isSync = false;

    explicit SetToolTip(const String& toolTip)
        : m_arguments(toolTip)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class SetFocus {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetFocus"); }
    static const bool isSync = false;

    explicit SetFocus(bool focused)
        : m_arguments(focused)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class TakeFocus {
public:
    typedef std::tuple<uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("TakeFocus"); }
    static const bool isSync = false;

    explicit TakeFocus(uint32_t direction)
        : m_arguments(direction)
    {
    }

    const std::tuple<uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t> m_arguments;
};

class FocusedFrameChanged {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("FocusedFrameChanged"); }
    static const bool isSync = false;

    explicit FocusedFrameChanged(uint64_t frameID)
        : m_arguments(frameID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class FrameSetLargestFrameChanged {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("FrameSetLargestFrameChanged"); }
    static const bool isSync = false;

    explicit FrameSetLargestFrameChanged(uint64_t frameID)
        : m_arguments(frameID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class SetRenderTreeSize {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetRenderTreeSize"); }
    static const bool isSync = false;

    explicit SetRenderTreeSize(uint64_t treeSize)
        : m_arguments(treeSize)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class SetToolbarsAreVisible {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetToolbarsAreVisible"); }
    static const bool isSync = false;

    explicit SetToolbarsAreVisible(bool toolbarsAreVisible)
        : m_arguments(toolbarsAreVisible)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class GetToolbarsAreVisible {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetToolbarsAreVisible"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class SetMenuBarIsVisible {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetMenuBarIsVisible"); }
    static const bool isSync = false;

    explicit SetMenuBarIsVisible(bool menuBarIsVisible)
        : m_arguments(menuBarIsVisible)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class GetMenuBarIsVisible {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetMenuBarIsVisible"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class SetStatusBarIsVisible {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetStatusBarIsVisible"); }
    static const bool isSync = false;

    explicit SetStatusBarIsVisible(bool statusBarIsVisible)
        : m_arguments(statusBarIsVisible)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class GetStatusBarIsVisible {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetStatusBarIsVisible"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class SetIsResizable {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetIsResizable"); }
    static const bool isSync = false;

    explicit SetIsResizable(bool isResizable)
        : m_arguments(isResizable)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class GetIsResizable {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetIsResizable"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class SetWindowFrame {
public:
    typedef std::tuple<WebCore::FloatRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetWindowFrame"); }
    static const bool isSync = false;

    explicit SetWindowFrame(const WebCore::FloatRect& windowFrame)
        : m_arguments(windowFrame)
    {
    }

    const std::tuple<const WebCore::FloatRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatRect&> m_arguments;
};

class GetWindowFrame {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetWindowFrame"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebCore::FloatRect&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class ScreenToRootView {
public:
    typedef std::tuple<WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ScreenToRootView"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebCore::IntPoint&> Reply;
    explicit ScreenToRootView(const WebCore::IntPoint& screenPoint)
        : m_arguments(screenPoint)
    {
    }

    const std::tuple<const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&> m_arguments;
};

class RootViewToScreen {
public:
    typedef std::tuple<WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RootViewToScreen"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebCore::IntRect&> Reply;
    explicit RootViewToScreen(const WebCore::IntRect& rect)
        : m_arguments(rect)
    {
    }

    const std::tuple<const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&> m_arguments;
};

#if PLATFORM(IOS)
class AccessibilityScreenToRootView {
public:
    typedef std::tuple<WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("AccessibilityScreenToRootView"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebCore::IntPoint&> Reply;
    explicit AccessibilityScreenToRootView(const WebCore::IntPoint& screenPoint)
        : m_arguments(screenPoint)
    {
    }

    const std::tuple<const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class RootViewToAccessibilityScreen {
public:
    typedef std::tuple<WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RootViewToAccessibilityScreen"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebCore::IntRect&> Reply;
    explicit RootViewToAccessibilityScreen(const WebCore::IntRect& rect)
        : m_arguments(rect)
    {
    }

    const std::tuple<const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&> m_arguments;
};
#endif

class RunBeforeUnloadConfirmPanel {
public:
    typedef std::tuple<String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunBeforeUnloadConfirmPanel"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    RunBeforeUnloadConfirmPanel(const String& message, uint64_t frameID)
        : m_arguments(message, frameID)
    {
    }

    const std::tuple<const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint64_t> m_arguments;
};

class PageDidScroll {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageDidScroll"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class RunOpenPanel {
public:
    typedef std::tuple<uint64_t, WebCore::FileChooserSettings> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunOpenPanel"); }
    static const bool isSync = false;

    RunOpenPanel(uint64_t frameID, const WebCore::FileChooserSettings& parameters)
        : m_arguments(frameID, parameters)
    {
    }

    const std::tuple<uint64_t, const WebCore::FileChooserSettings&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::FileChooserSettings&> m_arguments;
};

class PrintFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PrintFrame"); }
    static const bool isSync = true;

    typedef IPC::Arguments<> Reply;
    explicit PrintFrame(uint64_t frameID)
        : m_arguments(frameID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class RunModal {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RunModal"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class NotifyScrollerThumbIsVisibleInRect {
public:
    typedef std::tuple<WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("NotifyScrollerThumbIsVisibleInRect"); }
    static const bool isSync = false;

    explicit NotifyScrollerThumbIsVisibleInRect(const WebCore::IntRect& scrollerThumb)
        : m_arguments(scrollerThumb)
    {
    }

    const std::tuple<const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&> m_arguments;
};

class RecommendedScrollbarStyleDidChange {
public:
    typedef std::tuple<int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RecommendedScrollbarStyleDidChange"); }
    static const bool isSync = false;

    explicit RecommendedScrollbarStyleDidChange(int32_t newStyle)
        : m_arguments(newStyle)
    {
    }

    const std::tuple<int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t> m_arguments;
};

class DidChangeScrollbarsForMainFrame {
public:
    typedef std::tuple<bool, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangeScrollbarsForMainFrame"); }
    static const bool isSync = false;

    DidChangeScrollbarsForMainFrame(bool hasHorizontalScrollbar, bool hasVerticalScrollbar)
        : m_arguments(hasHorizontalScrollbar, hasVerticalScrollbar)
    {
    }

    const std::tuple<bool, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool, bool> m_arguments;
};

class DidChangeScrollOffsetPinningForMainFrame {
public:
    typedef std::tuple<bool, bool, bool, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangeScrollOffsetPinningForMainFrame"); }
    static const bool isSync = false;

    DidChangeScrollOffsetPinningForMainFrame(bool pinnedToLeftSide, bool pinnedToRightSide, bool pinnedToTopSide, bool pinnedToBottomSide)
        : m_arguments(pinnedToLeftSide, pinnedToRightSide, pinnedToTopSide, pinnedToBottomSide)
    {
    }

    const std::tuple<bool, bool, bool, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool, bool, bool, bool> m_arguments;
};

class DidChangePageCount {
public:
    typedef std::tuple<unsigned> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangePageCount"); }
    static const bool isSync = false;

    explicit DidChangePageCount(const unsigned& pageCount)
        : m_arguments(pageCount)
    {
    }

    const std::tuple<const unsigned&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const unsigned&> m_arguments;
};

class PageExtendedBackgroundColorDidChange {
public:
    typedef std::tuple<WebCore::Color> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageExtendedBackgroundColorDidChange"); }
    static const bool isSync = false;

    explicit PageExtendedBackgroundColorDidChange(const WebCore::Color& backgroundColor)
        : m_arguments(backgroundColor)
    {
    }

    const std::tuple<const WebCore::Color&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::Color&> m_arguments;
};

#if ENABLE(NETSCAPE_PLUGIN_API)
class DidFailToInitializePlugin {
public:
    typedef std::tuple<String, String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFailToInitializePlugin"); }
    static const bool isSync = false;

    DidFailToInitializePlugin(const String& mimeType, const String& frameURLString, const String& pageURLString)
        : m_arguments(mimeType, frameURLString, pageURLString)
    {
    }

    const std::tuple<const String&, const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const String&> m_arguments;
};
#endif

#if ENABLE(NETSCAPE_PLUGIN_API)
class DidBlockInsecurePluginVersion {
public:
    typedef std::tuple<String, String, String, String, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidBlockInsecurePluginVersion"); }
    static const bool isSync = false;

    DidBlockInsecurePluginVersion(const String& mimeType, const String& pluginURLString, const String& frameURLString, const String& pageURLString, bool replacementObscured)
        : m_arguments(mimeType, pluginURLString, frameURLString, pageURLString, replacementObscured)
    {
    }

    const std::tuple<const String&, const String&, const String&, const String&, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const String&, const String&, bool> m_arguments;
};
#endif

class SetCanShortCircuitHorizontalWheelEvents {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetCanShortCircuitHorizontalWheelEvents"); }
    static const bool isSync = false;

    explicit SetCanShortCircuitHorizontalWheelEvents(bool canShortCircuitHorizontalWheelEvents)
        : m_arguments(canShortCircuitHorizontalWheelEvents)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

#if PLATFORM(EFL)
class HandleInputMethodKeydown {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("HandleInputMethodKeydown"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if USE(TILED_BACKING_STORE)
class PageDidRequestScroll {
public:
    typedef std::tuple<WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageDidRequestScroll"); }
    static const bool isSync = false;

    explicit PageDidRequestScroll(const WebCore::IntPoint& point)
        : m_arguments(point)
    {
    }

    const std::tuple<const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&> m_arguments;
};
#endif

#if USE(TILED_BACKING_STORE)
class PageTransitionViewportReady {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageTransitionViewportReady"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if USE(COORDINATED_GRAPHICS)
class DidFindZoomableArea {
public:
    typedef std::tuple<WebCore::IntPoint, WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFindZoomableArea"); }
    static const bool isSync = false;

    DidFindZoomableArea(const WebCore::IntPoint& target, const WebCore::IntRect& area)
        : m_arguments(target, area)
    {
    }

    const std::tuple<const WebCore::IntPoint&, const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, const WebCore::IntRect&> m_arguments;
};
#endif

#if PLATFORM(EFL)
class DidChangeContentSize {
public:
    typedef std::tuple<WebCore::IntSize> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangeContentSize"); }
    static const bool isSync = false;

    explicit DidChangeContentSize(const WebCore::IntSize& newSize)
        : m_arguments(newSize)
    {
    }

    const std::tuple<const WebCore::IntSize&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntSize&> m_arguments;
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
class ShowColorPicker {
public:
    typedef std::tuple<WebCore::Color, WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowColorPicker"); }
    static const bool isSync = false;

    ShowColorPicker(const WebCore::Color& initialColor, const WebCore::IntRect& elementRect)
        : m_arguments(initialColor, elementRect)
    {
    }

    const std::tuple<const WebCore::Color&, const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::Color&, const WebCore::IntRect&> m_arguments;
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
class SetColorPickerColor {
public:
    typedef std::tuple<WebCore::Color> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetColorPickerColor"); }
    static const bool isSync = false;

    explicit SetColorPickerColor(const WebCore::Color& color)
        : m_arguments(color)
    {
    }

    const std::tuple<const WebCore::Color&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::Color&> m_arguments;
};
#endif

#if ENABLE(INPUT_TYPE_COLOR)
class EndColorPicker {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("EndColorPicker"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

class DecidePolicyForResponseSync {
public:
    typedef std::tuple<uint64_t, WebCore::ResourceResponse, WebCore::ResourceRequest, bool, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DecidePolicyForResponseSync"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&, uint64_t&, uint64_t&> Reply;
    DecidePolicyForResponseSync(uint64_t frameID, const WebCore::ResourceResponse& response, const WebCore::ResourceRequest& request, bool canShowMIMEType, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, response, request, canShowMIMEType, listenerID, userData)
    {
    }

    const std::tuple<uint64_t, const WebCore::ResourceResponse&, const WebCore::ResourceRequest&, bool, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::ResourceResponse&, const WebCore::ResourceRequest&, bool, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DecidePolicyForNavigationAction {
public:
    typedef std::tuple<uint64_t, uint64_t, WebKit::NavigationActionData, uint64_t, WebCore::ResourceRequest, WebCore::ResourceRequest, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DecidePolicyForNavigationAction"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&, uint64_t&, uint64_t&, uint64_t&> Reply;
    DecidePolicyForNavigationAction(uint64_t frameID, uint64_t navigationID, const WebKit::NavigationActionData& navigationActionData, uint64_t originatingFrameID, const WebCore::ResourceRequest& originalRequest, const WebCore::ResourceRequest& request, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, navigationActionData, originatingFrameID, originalRequest, request, listenerID, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const WebKit::NavigationActionData&, uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const WebKit::NavigationActionData&, uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DecidePolicyForNewWindowAction {
public:
    typedef std::tuple<uint64_t, WebKit::NavigationActionData, WebCore::ResourceRequest, String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DecidePolicyForNewWindowAction"); }
    static const bool isSync = false;

    DecidePolicyForNewWindowAction(uint64_t frameID, const WebKit::NavigationActionData& navigationActionData, const WebCore::ResourceRequest& request, const String& frameName, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationActionData, request, frameName, listenerID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::NavigationActionData&, const WebCore::ResourceRequest&, const String&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::NavigationActionData&, const WebCore::ResourceRequest&, const String&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class UnableToImplementPolicy {
public:
    typedef std::tuple<uint64_t, WebCore::ResourceError> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UnableToImplementPolicy"); }
    static const bool isSync = false;

    UnableToImplementPolicy(uint64_t frameID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, error, userData)
    {
    }

    const std::tuple<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidChangeProgress {
public:
    typedef std::tuple<double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidChangeProgress"); }
    static const bool isSync = false;

    explicit DidChangeProgress(double value)
        : m_arguments(value)
    {
    }

    const std::tuple<double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<double> m_arguments;
};

class DidFinishProgress {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishProgress"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class DidStartProgress {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidStartProgress"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class SetNetworkRequestsInProgress {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetNetworkRequestsInProgress"); }
    static const bool isSync = false;

    explicit SetNetworkRequestsInProgress(bool networkRequestsInProgress)
        : m_arguments(networkRequestsInProgress)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};

class DidCreateMainFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidCreateMainFrame"); }
    static const bool isSync = false;

    explicit DidCreateMainFrame(uint64_t frameID)
        : m_arguments(frameID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class DidCreateSubframe {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidCreateSubframe"); }
    static const bool isSync = false;

    explicit DidCreateSubframe(uint64_t frameID)
        : m_arguments(frameID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class DidStartProvisionalLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidStartProvisionalLoadForFrame"); }
    static const bool isSync = false;

    DidStartProvisionalLoadForFrame(uint64_t frameID, uint64_t navigationID, const String& url, const String& unreachableURL, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, url, unreachableURL, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const String&, const String&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const String&, const String&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidReceiveServerRedirectForProvisionalLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceiveServerRedirectForProvisionalLoadForFrame"); }
    static const bool isSync = false;

    DidReceiveServerRedirectForProvisionalLoadForFrame(uint64_t frameID, uint64_t navigationID, const String& url, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, url, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFailProvisionalLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, WebCore::ResourceError> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFailProvisionalLoadForFrame"); }
    static const bool isSync = false;

    DidFailProvisionalLoadForFrame(uint64_t frameID, uint64_t navigationID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, error, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidCommitLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, String, bool, uint32_t, WebCore::CertificateInfo> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidCommitLoadForFrame"); }
    static const bool isSync = false;

    DidCommitLoadForFrame(uint64_t frameID, uint64_t navigationID, const String& mimeType, bool hasCustomContentProvider, uint32_t loadType, const WebCore::CertificateInfo& certificateInfo, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, mimeType, hasCustomContentProvider, loadType, certificateInfo, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const String&, bool, uint32_t, const WebCore::CertificateInfo&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const String&, bool, uint32_t, const WebCore::CertificateInfo&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFailLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, WebCore::ResourceError> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFailLoadForFrame"); }
    static const bool isSync = false;

    DidFailLoadForFrame(uint64_t frameID, uint64_t navigationID, const WebCore::ResourceError& error, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, error, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const WebCore::ResourceError&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFinishDocumentLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishDocumentLoadForFrame"); }
    static const bool isSync = false;

    DidFinishDocumentLoadForFrame(uint64_t frameID, uint64_t navigationID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFinishLoadForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishLoadForFrame"); }
    static const bool isSync = false;

    DidFinishLoadForFrame(uint64_t frameID, uint64_t navigationID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFirstLayoutForFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFirstLayoutForFrame"); }
    static const bool isSync = false;

    DidFirstLayoutForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidFirstVisuallyNonEmptyLayoutForFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFirstVisuallyNonEmptyLayoutForFrame"); }
    static const bool isSync = false;

    DidFirstVisuallyNonEmptyLayoutForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidLayout {
public:
    typedef std::tuple<uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidLayout"); }
    static const bool isSync = false;

    DidLayout(uint32_t type, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(type, userData)
    {
    }

    const std::tuple<uint32_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidRemoveFrameFromHierarchy {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidRemoveFrameFromHierarchy"); }
    static const bool isSync = false;

    DidRemoveFrameFromHierarchy(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidReceiveTitleForFrame {
public:
    typedef std::tuple<uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceiveTitleForFrame"); }
    static const bool isSync = false;

    DidReceiveTitleForFrame(uint64_t frameID, const String& title, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, title, userData)
    {
    }

    const std::tuple<uint64_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidDisplayInsecureContentForFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidDisplayInsecureContentForFrame"); }
    static const bool isSync = false;

    DidDisplayInsecureContentForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidRunInsecureContentForFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidRunInsecureContentForFrame"); }
    static const bool isSync = false;

    DidRunInsecureContentForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidDetectXSSForFrame {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidDetectXSSForFrame"); }
    static const bool isSync = false;

    DidDetectXSSForFrame(uint64_t frameID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidSameDocumentNavigationForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, uint32_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidSameDocumentNavigationForFrame"); }
    static const bool isSync = false;

    DidSameDocumentNavigationForFrame(uint64_t frameID, uint64_t navigationID, uint32_t type, const String& url, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, navigationID, type, url, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, uint32_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, uint32_t, const String&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class DidDestroyNavigation {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidDestroyNavigation"); }
    static const bool isSync = false;

    explicit DidDestroyNavigation(uint64_t navigationID)
        : m_arguments(navigationID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class FrameDidBecomeFrameSet {
public:
    typedef std::tuple<uint64_t, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("FrameDidBecomeFrameSet"); }
    static const bool isSync = false;

    FrameDidBecomeFrameSet(uint64_t frameID, bool value)
        : m_arguments(frameID, value)
    {
    }

    const std::tuple<uint64_t, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, bool> m_arguments;
};

class DidFinishLoadingDataForCustomContentProvider {
public:
    typedef std::tuple<String, IPC::DataReference> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishLoadingDataForCustomContentProvider"); }
    static const bool isSync = false;

    DidFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference& data)
        : m_arguments(suggestedFilename, data)
    {
    }

    const std::tuple<const String&, const IPC::DataReference&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const IPC::DataReference&> m_arguments;
};

class WillSubmitForm {
public:
    typedef std::tuple<uint64_t, uint64_t, Vector<std::pair<String, String>>, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WillSubmitForm"); }
    static const bool isSync = false;

    WillSubmitForm(uint64_t frameID, uint64_t sourceFrameID, const Vector<std::pair<String, String>>& textFieldValues, uint64_t listenerID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(frameID, sourceFrameID, textFieldValues, listenerID, userData)
    {
    }

    const std::tuple<uint64_t, uint64_t, const Vector<std::pair<String, String>>&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const Vector<std::pair<String, String>>&, uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class VoidCallback {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("VoidCallback"); }
    static const bool isSync = false;

    explicit VoidCallback(uint64_t callbackID)
        : m_arguments(callbackID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class DataCallback {
public:
    typedef std::tuple<IPC::DataReference, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DataCallback"); }
    static const bool isSync = false;

    DataCallback(const IPC::DataReference& resultData, uint64_t callbackID)
        : m_arguments(resultData, callbackID)
    {
    }

    const std::tuple<const IPC::DataReference&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const IPC::DataReference&, uint64_t> m_arguments;
};

class ImageCallback {
public:
    typedef std::tuple<WebKit::ShareableBitmap::Handle, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ImageCallback"); }
    static const bool isSync = false;

    ImageCallback(const WebKit::ShareableBitmap::Handle& bitmapHandle, uint64_t callbackID)
        : m_arguments(bitmapHandle, callbackID)
    {
    }

    const std::tuple<const WebKit::ShareableBitmap::Handle&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::ShareableBitmap::Handle&, uint64_t> m_arguments;
};

class StringCallback {
public:
    typedef std::tuple<String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StringCallback"); }
    static const bool isSync = false;

    StringCallback(const String& resultString, uint64_t callbackID)
        : m_arguments(resultString, callbackID)
    {
    }

    const std::tuple<const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint64_t> m_arguments;
};

class ScriptValueCallback {
public:
    typedef std::tuple<IPC::DataReference, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ScriptValueCallback"); }
    static const bool isSync = false;

    ScriptValueCallback(const IPC::DataReference& resultData, uint64_t callbackID)
        : m_arguments(resultData, callbackID)
    {
    }

    const std::tuple<const IPC::DataReference&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const IPC::DataReference&, uint64_t> m_arguments;
};

class ComputedPagesCallback {
public:
    typedef std::tuple<Vector<WebCore::IntRect>, double, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ComputedPagesCallback"); }
    static const bool isSync = false;

    ComputedPagesCallback(const Vector<WebCore::IntRect>& pageRects, double totalScaleFactorForPrinting, uint64_t callbackID)
        : m_arguments(pageRects, totalScaleFactorForPrinting, callbackID)
    {
    }

    const std::tuple<const Vector<WebCore::IntRect>&, double, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const Vector<WebCore::IntRect>&, double, uint64_t> m_arguments;
};

class ValidateCommandCallback {
public:
    typedef std::tuple<String, bool, int32_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ValidateCommandCallback"); }
    static const bool isSync = false;

    ValidateCommandCallback(const String& command, bool isEnabled, int32_t state, uint64_t callbackID)
        : m_arguments(command, isEnabled, state, callbackID)
    {
    }

    const std::tuple<const String&, bool, int32_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, bool, int32_t, uint64_t> m_arguments;
};

class EditingRangeCallback {
public:
    typedef std::tuple<WebKit::EditingRange, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("EditingRangeCallback"); }
    static const bool isSync = false;

    EditingRangeCallback(const WebKit::EditingRange& range, uint64_t callbackID)
        : m_arguments(range, callbackID)
    {
    }

    const std::tuple<const WebKit::EditingRange&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::EditingRange&, uint64_t> m_arguments;
};

class UnsignedCallback {
public:
    typedef std::tuple<uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UnsignedCallback"); }
    static const bool isSync = false;

    UnsignedCallback(uint64_t result, uint64_t callbackID)
        : m_arguments(result, callbackID)
    {
    }

    const std::tuple<uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t> m_arguments;
};

class RectForCharacterRangeCallback {
public:
    typedef std::tuple<WebCore::IntRect, WebKit::EditingRange, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RectForCharacterRangeCallback"); }
    static const bool isSync = false;

    RectForCharacterRangeCallback(const WebCore::IntRect& rect, const WebKit::EditingRange& actualRange, uint64_t callbackID)
        : m_arguments(rect, actualRange, callbackID)
    {
    }

    const std::tuple<const WebCore::IntRect&, const WebKit::EditingRange&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&, const WebKit::EditingRange&, uint64_t> m_arguments;
};

#if PLATFORM(MAC)
class AttributedStringForCharacterRangeCallback {
public:
    typedef std::tuple<WebKit::AttributedString, WebKit::EditingRange, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("AttributedStringForCharacterRangeCallback"); }
    static const bool isSync = false;

    AttributedStringForCharacterRangeCallback(const WebKit::AttributedString& string, const WebKit::EditingRange& actualRange, uint64_t callbackID)
        : m_arguments(string, actualRange, callbackID)
    {
    }

    const std::tuple<const WebKit::AttributedString&, const WebKit::EditingRange&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::AttributedString&, const WebKit::EditingRange&, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class GestureCallback {
public:
    typedef std::tuple<WebCore::IntPoint, uint32_t, uint32_t, uint32_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GestureCallback"); }
    static const bool isSync = false;

    GestureCallback(const WebCore::IntPoint& point, uint32_t gestureType, uint32_t gestureState, uint32_t flags, uint64_t callbackID)
        : m_arguments(point, gestureType, gestureState, flags, callbackID)
    {
    }

    const std::tuple<const WebCore::IntPoint&, uint32_t, uint32_t, uint32_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, uint32_t, uint32_t, uint32_t, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class TouchesCallback {
public:
    typedef std::tuple<WebCore::IntPoint, uint32_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("TouchesCallback"); }
    static const bool isSync = false;

    TouchesCallback(const WebCore::IntPoint& point, uint32_t touches, uint64_t callbackID)
        : m_arguments(point, touches, callbackID)
    {
    }

    const std::tuple<const WebCore::IntPoint&, uint32_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, uint32_t, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class AutocorrectionDataCallback {
public:
    typedef std::tuple<Vector<WebCore::FloatRect>, String, double, uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("AutocorrectionDataCallback"); }
    static const bool isSync = false;

    AutocorrectionDataCallback(const Vector<WebCore::FloatRect>& textRects, const String& fontName, double fontSize, uint64_t traits, uint64_t callbackID)
        : m_arguments(textRects, fontName, fontSize, traits, callbackID)
    {
    }

    const std::tuple<const Vector<WebCore::FloatRect>&, const String&, double, uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const Vector<WebCore::FloatRect>&, const String&, double, uint64_t, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class AutocorrectionContextCallback {
public:
    typedef std::tuple<String, String, String, String, uint64_t, uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("AutocorrectionContextCallback"); }
    static const bool isSync = false;

    AutocorrectionContextCallback(const String& beforeText, const String& markedText, const String& selectedText, const String& afterText, uint64_t location, uint64_t length, uint64_t callbackID)
        : m_arguments(beforeText, markedText, selectedText, afterText, location, length, callbackID)
    {
    }

    const std::tuple<const String&, const String&, const String&, const String&, uint64_t, uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const String&, const String&, uint64_t, uint64_t, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DictationContextCallback {
public:
    typedef std::tuple<String, String, String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DictationContextCallback"); }
    static const bool isSync = false;

    DictationContextCallback(const String& selectedText, const String& beforeText, const String& afterText, uint64_t callbackID)
        : m_arguments(selectedText, beforeText, afterText, callbackID)
    {
    }

    const std::tuple<const String&, const String&, const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const String&, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class InterpretKeyEvent {
public:
    typedef std::tuple<WebKit::EditorState, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("InterpretKeyEvent"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    InterpretKeyEvent(const WebKit::EditorState& state, bool isCharEvent)
        : m_arguments(state, isCharEvent)
    {
    }

    const std::tuple<const WebKit::EditorState&, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::EditorState&, bool> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DidReceivePositionInformation {
public:
    typedef std::tuple<WebKit::InteractionInformationAtPosition> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceivePositionInformation"); }
    static const bool isSync = false;

    explicit DidReceivePositionInformation(const WebKit::InteractionInformationAtPosition& information)
        : m_arguments(information)
    {
    }

    const std::tuple<const WebKit::InteractionInformationAtPosition&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::InteractionInformationAtPosition&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class SaveImageToLibrary {
public:
    typedef std::tuple<WebKit::SharedMemory::Handle, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SaveImageToLibrary"); }
    static const bool isSync = false;

    SaveImageToLibrary(const WebKit::SharedMemory::Handle& handle, uint64_t size)
        : m_arguments(handle, size)
    {
    }

    const std::tuple<const WebKit::SharedMemory::Handle&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::SharedMemory::Handle&, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DidUpdateBlockSelectionWithTouch {
public:
    typedef std::tuple<uint32_t, uint32_t, float, float> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidUpdateBlockSelectionWithTouch"); }
    static const bool isSync = false;

    DidUpdateBlockSelectionWithTouch(uint32_t touch, uint32_t flags, float growThreshold, float shrinkThreshold)
        : m_arguments(touch, flags, growThreshold, shrinkThreshold)
    {
    }

    const std::tuple<uint32_t, uint32_t, float, float>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t, uint32_t, float, float> m_arguments;
};
#endif

#if PLATFORM(IOS)
class ShowPlaybackTargetPicker {
public:
    typedef std::tuple<bool, WebCore::IntRect> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowPlaybackTargetPicker"); }
    static const bool isSync = false;

    ShowPlaybackTargetPicker(bool hasVideo, const WebCore::IntRect& elementRect)
        : m_arguments(hasVideo, elementRect)
    {
    }

    const std::tuple<bool, const WebCore::IntRect&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool, const WebCore::IntRect&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class ZoomToRect {
public:
    typedef std::tuple<WebCore::FloatRect, double, double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ZoomToRect"); }
    static const bool isSync = false;

    ZoomToRect(const WebCore::FloatRect& rect, double minimumScale, double maximumScale)
        : m_arguments(rect, minimumScale, maximumScale)
    {
    }

    const std::tuple<const WebCore::FloatRect&, double, double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatRect&, double, double> m_arguments;
};
#endif

#if PLATFORM(IOS)
class CommitPotentialTapFailed {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CommitPotentialTapFailed"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DidNotHandleTapAsClick {
public:
    typedef std::tuple<WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidNotHandleTapAsClick"); }
    static const bool isSync = false;

    explicit DidNotHandleTapAsClick(const WebCore::IntPoint& point)
        : m_arguments(point)
    {
    }

    const std::tuple<const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class ViewportMetaTagWidthDidChange {
public:
    typedef std::tuple<float> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ViewportMetaTagWidthDidChange"); }
    static const bool isSync = false;

    explicit ViewportMetaTagWidthDidChange(float width)
        : m_arguments(width)
    {
    }

    const std::tuple<float>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<float> m_arguments;
};
#endif

#if PLATFORM(IOS)
class SetUsesMinimalUI {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetUsesMinimalUI"); }
    static const bool isSync = false;

    explicit SetUsesMinimalUI(bool usesMinimalUI)
        : m_arguments(usesMinimalUI)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DidFinishDrawingPagesToPDF {
public:
    typedef std::tuple<IPC::DataReference> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishDrawingPagesToPDF"); }
    static const bool isSync = false;

    explicit DidFinishDrawingPagesToPDF(const IPC::DataReference& pdfData)
        : m_arguments(pdfData)
    {
    }

    const std::tuple<const IPC::DataReference&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const IPC::DataReference&> m_arguments;
};
#endif

#if PLATFORM(GTK)
class PrintFinishedCallback {
public:
    typedef std::tuple<WebCore::ResourceError, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PrintFinishedCallback"); }
    static const bool isSync = false;

    PrintFinishedCallback(const WebCore::ResourceError& error, uint64_t callbackID)
        : m_arguments(error, callbackID)
    {
    }

    const std::tuple<const WebCore::ResourceError&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::ResourceError&, uint64_t> m_arguments;
};
#endif

class PageScaleFactorDidChange {
public:
    typedef std::tuple<double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageScaleFactorDidChange"); }
    static const bool isSync = false;

    explicit PageScaleFactorDidChange(double scaleFactor)
        : m_arguments(scaleFactor)
    {
    }

    const std::tuple<double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<double> m_arguments;
};

class PageZoomFactorDidChange {
public:
    typedef std::tuple<double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PageZoomFactorDidChange"); }
    static const bool isSync = false;

    explicit PageZoomFactorDidChange(double zoomFactor)
        : m_arguments(zoomFactor)
    {
    }

    const std::tuple<double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<double> m_arguments;
};

#if PLATFORM(GTK)
class GetEditorCommandsForKeyEvent {
public:
    typedef std::tuple<AtomicString> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetEditorCommandsForKeyEvent"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<String>&> Reply;
    explicit GetEditorCommandsForKeyEvent(const AtomicString& eventType)
        : m_arguments(eventType)
    {
    }

    const std::tuple<const AtomicString&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const AtomicString&> m_arguments;
};
#endif

#if PLATFORM(GTK)
class BindAccessibilityTree {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BindAccessibilityTree"); }
    static const bool isSync = false;

    explicit BindAccessibilityTree(const String& plugID)
        : m_arguments(plugID)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if PLATFORM(GTK)
class SetInputMethodState {
public:
    typedef std::tuple<bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetInputMethodState"); }
    static const bool isSync = false;

    explicit SetInputMethodState(bool enabled)
        : m_arguments(enabled)
    {
    }

    const std::tuple<bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool> m_arguments;
};
#endif

class BackForwardAddItem {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardAddItem"); }
    static const bool isSync = false;

    explicit BackForwardAddItem(uint64_t itemID)
        : m_arguments(itemID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class BackForwardGoToItem {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardGoToItem"); }
    static const bool isSync = true;

    typedef IPC::Arguments<WebKit::SandboxExtension::Handle&> Reply;
    explicit BackForwardGoToItem(uint64_t itemID)
        : m_arguments(itemID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class BackForwardItemAtIndex {
public:
    typedef std::tuple<int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardItemAtIndex"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint64_t&> Reply;
    explicit BackForwardItemAtIndex(int32_t itemIndex)
        : m_arguments(itemIndex)
    {
    }

    const std::tuple<int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t> m_arguments;
};

class BackForwardBackListCount {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardBackListCount"); }
    static const bool isSync = true;

    typedef IPC::Arguments<int32_t&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class BackForwardForwardListCount {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardForwardListCount"); }
    static const bool isSync = true;

    typedef IPC::Arguments<int32_t&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class BackForwardClear {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("BackForwardClear"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class WillGoToBackForwardListItem {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WillGoToBackForwardListItem"); }
    static const bool isSync = false;

    WillGoToBackForwardListItem(uint64_t itemID, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(itemID, userData)
    {
    }

    const std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};

class RegisterEditCommandForUndo {
public:
    typedef std::tuple<uint64_t, uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RegisterEditCommandForUndo"); }
    static const bool isSync = false;

    RegisterEditCommandForUndo(uint64_t commandID, uint32_t editAction)
        : m_arguments(commandID, editAction)
    {
    }

    const std::tuple<uint64_t, uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint32_t> m_arguments;
};

class ClearAllEditCommands {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ClearAllEditCommands"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class RegisterInsertionUndoGrouping {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RegisterInsertionUndoGrouping"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class CanUndoRedo {
public:
    typedef std::tuple<uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CanUndoRedo"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    explicit CanUndoRedo(uint32_t action)
        : m_arguments(action)
    {
    }

    const std::tuple<uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t> m_arguments;
};

class ExecuteUndoRedo {
public:
    typedef std::tuple<uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ExecuteUndoRedo"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    explicit ExecuteUndoRedo(uint32_t action)
        : m_arguments(action)
    {
    }

    const std::tuple<uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint32_t> m_arguments;
};

class EditorStateChanged {
public:
    typedef std::tuple<WebKit::EditorState> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("EditorStateChanged"); }
    static const bool isSync = false;

    explicit EditorStateChanged(const WebKit::EditorState& editorState)
        : m_arguments(editorState)
    {
    }

    const std::tuple<const WebKit::EditorState&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::EditorState&> m_arguments;
};

class CompositionWasCanceled {
public:
    typedef std::tuple<WebKit::EditorState> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CompositionWasCanceled"); }
    static const bool isSync = false;

    explicit CompositionWasCanceled(const WebKit::EditorState& editorState)
        : m_arguments(editorState)
    {
    }

    const std::tuple<const WebKit::EditorState&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::EditorState&> m_arguments;
};

class DidCountStringMatches {
public:
    typedef std::tuple<String, uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidCountStringMatches"); }
    static const bool isSync = false;

    DidCountStringMatches(const String& string, uint32_t matchCount)
        : m_arguments(string, matchCount)
    {
    }

    const std::tuple<const String&, uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint32_t> m_arguments;
};

class SetFindIndicator {
public:
    typedef std::tuple<WebCore::FloatRect, Vector<WebCore::FloatRect>, float, WebKit::ShareableBitmap::Handle, bool, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetFindIndicator"); }
    static const bool isSync = false;

    SetFindIndicator(const WebCore::FloatRect& selectionRect, const Vector<WebCore::FloatRect>& textRects, float contentImageScaleFactor, const WebKit::ShareableBitmap::Handle& contentImageHandle, bool fadeOut, bool animate)
        : m_arguments(selectionRect, textRects, contentImageScaleFactor, contentImageHandle, fadeOut, animate)
    {
    }

    const std::tuple<const WebCore::FloatRect&, const Vector<WebCore::FloatRect>&, float, const WebKit::ShareableBitmap::Handle&, bool, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatRect&, const Vector<WebCore::FloatRect>&, float, const WebKit::ShareableBitmap::Handle&, bool, bool> m_arguments;
};

class DidFindString {
public:
    typedef std::tuple<String, uint32_t, int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFindString"); }
    static const bool isSync = false;

    DidFindString(const String& string, uint32_t matchCount, int32_t matchIndex)
        : m_arguments(string, matchCount, matchIndex)
    {
    }

    const std::tuple<const String&, uint32_t, int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint32_t, int32_t> m_arguments;
};

class DidFailToFindString {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFailToFindString"); }
    static const bool isSync = false;

    explicit DidFailToFindString(const String& string)
        : m_arguments(string)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class DidFindStringMatches {
public:
    typedef std::tuple<String, Vector<Vector<WebCore::IntRect>>, int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFindStringMatches"); }
    static const bool isSync = false;

    DidFindStringMatches(const String& string, const Vector<Vector<WebCore::IntRect>>& matches, int32_t firstIndexAfterSelection)
        : m_arguments(string, matches, firstIndexAfterSelection)
    {
    }

    const std::tuple<const String&, const Vector<Vector<WebCore::IntRect>>&, int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const Vector<Vector<WebCore::IntRect>>&, int32_t> m_arguments;
};

class DidGetImageForFindMatch {
public:
    typedef std::tuple<WebKit::ShareableBitmap::Handle, uint32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidGetImageForFindMatch"); }
    static const bool isSync = false;

    DidGetImageForFindMatch(const WebKit::ShareableBitmap::Handle& contentImageHandle, uint32_t matchIndex)
        : m_arguments(contentImageHandle, matchIndex)
    {
    }

    const std::tuple<const WebKit::ShareableBitmap::Handle&, uint32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::ShareableBitmap::Handle&, uint32_t> m_arguments;
};

class ShowPopupMenu {
public:
    typedef std::tuple<WebCore::IntRect, uint64_t, Vector<WebKit::WebPopupItem>, int32_t, WebKit::PlatformPopupMenuData> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowPopupMenu"); }
    static const bool isSync = false;

    ShowPopupMenu(const WebCore::IntRect& rect, uint64_t textDirection, const Vector<WebKit::WebPopupItem>& items, int32_t selectedIndex, const WebKit::PlatformPopupMenuData& data)
        : m_arguments(rect, textDirection, items, selectedIndex, data)
    {
    }

    const std::tuple<const WebCore::IntRect&, uint64_t, const Vector<WebKit::WebPopupItem>&, int32_t, const WebKit::PlatformPopupMenuData&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&, uint64_t, const Vector<WebKit::WebPopupItem>&, int32_t, const WebKit::PlatformPopupMenuData&> m_arguments;
};

class HidePopupMenu {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("HidePopupMenu"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

#if ENABLE(CONTEXT_MENUS)
class ShowContextMenu {
public:
    typedef std::tuple<WebCore::IntPoint, WebKit::ContextMenuContextData, Vector<WebKit::WebContextMenuItemData>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowContextMenu"); }
    static const bool isSync = false;

    ShowContextMenu(const WebCore::IntPoint& menuLocation, const WebKit::ContextMenuContextData& contextMenuContextData, const Vector<WebKit::WebContextMenuItemData>& items, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(menuLocation, contextMenuContextData, items, userData)
    {
    }

    const std::tuple<const WebCore::IntPoint&, const WebKit::ContextMenuContextData&, const Vector<WebKit::WebContextMenuItemData>&, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, const WebKit::ContextMenuContextData&, const Vector<WebKit::WebContextMenuItemData>&, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};
#endif

class CanAuthenticateAgainstProtectionSpaceInFrame {
public:
    typedef std::tuple<uint64_t, WebCore::ProtectionSpace> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CanAuthenticateAgainstProtectionSpaceInFrame"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    CanAuthenticateAgainstProtectionSpaceInFrame(uint64_t frameID, const WebCore::ProtectionSpace& protectionSpace)
        : m_arguments(frameID, protectionSpace)
    {
    }

    const std::tuple<uint64_t, const WebCore::ProtectionSpace&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::ProtectionSpace&> m_arguments;
};

class DidReceiveAuthenticationChallenge {
public:
    typedef std::tuple<uint64_t, WebCore::AuthenticationChallenge, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceiveAuthenticationChallenge"); }
    static const bool isSync = false;

    DidReceiveAuthenticationChallenge(uint64_t frameID, const WebCore::AuthenticationChallenge& challenge, uint64_t challengeID)
        : m_arguments(frameID, challenge, challengeID)
    {
    }

    const std::tuple<uint64_t, const WebCore::AuthenticationChallenge&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::AuthenticationChallenge&, uint64_t> m_arguments;
};

class ExceededDatabaseQuota {
public:
    typedef std::tuple<uint64_t, String, String, String, uint64_t, uint64_t, uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ExceededDatabaseQuota"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send(uint64_t newQuota);

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<uint64_t&> Reply;
    ExceededDatabaseQuota(uint64_t frameID, const String& originIdentifier, const String& databaseName, const String& databaseDisplayName, uint64_t currentQuota, uint64_t currentOriginUsage, uint64_t currentDatabaseUsage, uint64_t expectedUsage)
        : m_arguments(frameID, originIdentifier, databaseName, databaseDisplayName, currentQuota, currentOriginUsage, currentDatabaseUsage, expectedUsage)
    {
    }

    const std::tuple<uint64_t, const String&, const String&, const String&, uint64_t, uint64_t, uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&, const String&, const String&, uint64_t, uint64_t, uint64_t, uint64_t> m_arguments;
};

class ReachedApplicationCacheOriginQuota {
public:
    typedef std::tuple<String, uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ReachedApplicationCacheOriginQuota"); }
    static const bool isSync = true;

    struct DelayedReply : public ThreadSafeRefCounted<DelayedReply> {
        DelayedReply(PassRefPtr<IPC::Connection>, std::unique_ptr<IPC::MessageEncoder>);
        ~DelayedReply();

        bool send(uint64_t newQuota);

    private:
        RefPtr<IPC::Connection> m_connection;
        std::unique_ptr<IPC::MessageEncoder> m_encoder;
    };

    typedef IPC::Arguments<uint64_t&> Reply;
    ReachedApplicationCacheOriginQuota(const String& originIdentifier, uint64_t currentQuota, uint64_t totalBytesNeeded)
        : m_arguments(originIdentifier, currentQuota, totalBytesNeeded)
    {
    }

    const std::tuple<const String&, uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint64_t, uint64_t> m_arguments;
};

class RequestGeolocationPermissionForFrame {
public:
    typedef std::tuple<uint64_t, uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RequestGeolocationPermissionForFrame"); }
    static const bool isSync = false;

    RequestGeolocationPermissionForFrame(uint64_t geolocationID, uint64_t frameID, const String& originIdentifier)
        : m_arguments(geolocationID, frameID, originIdentifier)
    {
    }

    const std::tuple<uint64_t, uint64_t, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t, const String&> m_arguments;
};

class RequestNotificationPermission {
public:
    typedef std::tuple<uint64_t, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RequestNotificationPermission"); }
    static const bool isSync = false;

    RequestNotificationPermission(uint64_t requestID, const String& originIdentifier)
        : m_arguments(requestID, originIdentifier)
    {
    }

    const std::tuple<uint64_t, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const String&> m_arguments;
};

class ShowNotification {
public:
    typedef std::tuple<String, String, String, String, String, String, String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowNotification"); }
    static const bool isSync = false;

    ShowNotification(const String& title, const String& body, const String& iconURL, const String& tag, const String& lang, const String& dir, const String& originIdentifier, uint64_t notificationID)
        : m_arguments(title, body, iconURL, tag, lang, dir, originIdentifier, notificationID)
    {
    }

    const std::tuple<const String&, const String&, const String&, const String&, const String&, const String&, const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const String&, const String&, const String&, const String&, const String&, uint64_t> m_arguments;
};

class CancelNotification {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CancelNotification"); }
    static const bool isSync = false;

    explicit CancelNotification(uint64_t notificationID)
        : m_arguments(notificationID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

class ClearNotifications {
public:
    typedef std::tuple<Vector<uint64_t>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ClearNotifications"); }
    static const bool isSync = false;

    explicit ClearNotifications(const Vector<uint64_t>& notificationIDs)
        : m_arguments(notificationIDs)
    {
    }

    const std::tuple<const Vector<uint64_t>&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const Vector<uint64_t>&> m_arguments;
};

class DidDestroyNotification {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidDestroyNotification"); }
    static const bool isSync = false;

    explicit DidDestroyNotification(uint64_t notificationID)
        : m_arguments(notificationID)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};

#if USE(UNIFIED_TEXT_CHECKING)
class CheckTextOfParagraph {
public:
    typedef std::tuple<String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CheckTextOfParagraph"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<WebCore::TextCheckingResult>&> Reply;
    CheckTextOfParagraph(const String& text, uint64_t checkingTypes)
        : m_arguments(text, checkingTypes)
    {
    }

    const std::tuple<const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint64_t> m_arguments;
};
#endif

class CheckSpellingOfString {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CheckSpellingOfString"); }
    static const bool isSync = true;

    typedef IPC::Arguments<int32_t&, int32_t&> Reply;
    explicit CheckSpellingOfString(const String& text)
        : m_arguments(text)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class CheckGrammarOfString {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CheckGrammarOfString"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<WebCore::GrammarDetail>&, int32_t&, int32_t&> Reply;
    explicit CheckGrammarOfString(const String& text)
        : m_arguments(text)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class SpellingUIIsShowing {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SpellingUIIsShowing"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class UpdateSpellingUIWithMisspelledWord {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UpdateSpellingUIWithMisspelledWord"); }
    static const bool isSync = false;

    explicit UpdateSpellingUIWithMisspelledWord(const String& misspelledWord)
        : m_arguments(misspelledWord)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class UpdateSpellingUIWithGrammarString {
public:
    typedef std::tuple<String, WebCore::GrammarDetail> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UpdateSpellingUIWithGrammarString"); }
    static const bool isSync = false;

    UpdateSpellingUIWithGrammarString(const String& badGrammarPhrase, const WebCore::GrammarDetail& grammarDetail)
        : m_arguments(badGrammarPhrase, grammarDetail)
    {
    }

    const std::tuple<const String&, const WebCore::GrammarDetail&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const WebCore::GrammarDetail&> m_arguments;
};

class GetGuessesForWord {
public:
    typedef std::tuple<String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetGuessesForWord"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<String>&> Reply;
    GetGuessesForWord(const String& word, const String& context)
        : m_arguments(word, context)
    {
    }

    const std::tuple<const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&> m_arguments;
};

class LearnWord {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("LearnWord"); }
    static const bool isSync = false;

    explicit LearnWord(const String& word)
        : m_arguments(word)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class IgnoreWord {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("IgnoreWord"); }
    static const bool isSync = false;

    explicit IgnoreWord(const String& word)
        : m_arguments(word)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

class RequestCheckingOfString {
public:
    typedef std::tuple<uint64_t, WebCore::TextCheckingRequestData> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RequestCheckingOfString"); }
    static const bool isSync = false;

    RequestCheckingOfString(uint64_t requestID, const WebCore::TextCheckingRequestData& request)
        : m_arguments(requestID, request)
    {
    }

    const std::tuple<uint64_t, const WebCore::TextCheckingRequestData&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::TextCheckingRequestData&> m_arguments;
};

#if ENABLE(DRAG_SUPPORT)
class DidPerformDragControllerAction {
public:
    typedef std::tuple<uint64_t, bool, unsigned> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidPerformDragControllerAction"); }
    static const bool isSync = false;

    DidPerformDragControllerAction(uint64_t dragOperation, bool mouseIsOverFileInput, const unsigned& numberOfItemsToBeAccepted)
        : m_arguments(dragOperation, mouseIsOverFileInput, numberOfItemsToBeAccepted)
    {
    }

    const std::tuple<uint64_t, bool, const unsigned&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, bool, const unsigned&> m_arguments;
};
#endif

#if PLATFORM(COCOA) && ENABLE(DRAG_SUPPORT)
class SetDragImage {
public:
    typedef std::tuple<WebCore::IntPoint, WebKit::ShareableBitmap::Handle, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetDragImage"); }
    static const bool isSync = false;

    SetDragImage(const WebCore::IntPoint& clientPosition, const WebKit::ShareableBitmap::Handle& dragImage, bool linkDrag)
        : m_arguments(clientPosition, dragImage, linkDrag)
    {
    }

    const std::tuple<const WebCore::IntPoint&, const WebKit::ShareableBitmap::Handle&, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntPoint&, const WebKit::ShareableBitmap::Handle&, bool> m_arguments;
};
#endif

#if PLATFORM(COCOA) && ENABLE(DRAG_SUPPORT)
class SetPromisedData {
public:
    typedef std::tuple<String, WebKit::SharedMemory::Handle, uint64_t, String, String, String, String, String, WebKit::SharedMemory::Handle, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetPromisedData"); }
    static const bool isSync = false;

    SetPromisedData(const String& pasteboardName, const WebKit::SharedMemory::Handle& imageHandle, uint64_t imageSize, const String& filename, const String& extension, const String& title, const String& url, const String& visibleURL, const WebKit::SharedMemory::Handle& archiveHandle, uint64_t archiveSize)
        : m_arguments(pasteboardName, imageHandle, imageSize, filename, extension, title, url, visibleURL, archiveHandle, archiveSize)
    {
    }

    const std::tuple<const String&, const WebKit::SharedMemory::Handle&, uint64_t, const String&, const String&, const String&, const String&, const String&, const WebKit::SharedMemory::Handle&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const WebKit::SharedMemory::Handle&, uint64_t, const String&, const String&, const String&, const String&, const String&, const WebKit::SharedMemory::Handle&, uint64_t> m_arguments;
};
#endif

#if PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
class StartDrag {
public:
    typedef std::tuple<WebCore::DragData, WebKit::ShareableBitmap::Handle> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StartDrag"); }
    static const bool isSync = false;

    StartDrag(const WebCore::DragData& dragData, const WebKit::ShareableBitmap::Handle& dragImage)
        : m_arguments(dragData, dragImage)
    {
    }

    const std::tuple<const WebCore::DragData&, const WebKit::ShareableBitmap::Handle&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::DragData&, const WebKit::ShareableBitmap::Handle&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class DidPerformDictionaryLookup {
public:
    typedef std::tuple<WebKit::AttributedString, WebKit::DictionaryPopupInfo> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidPerformDictionaryLookup"); }
    static const bool isSync = false;

    DidPerformDictionaryLookup(const WebKit::AttributedString& text, const WebKit::DictionaryPopupInfo& dictionaryPopupInfo)
        : m_arguments(text, dictionaryPopupInfo)
    {
    }

    const std::tuple<const WebKit::AttributedString&, const WebKit::DictionaryPopupInfo&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::AttributedString&, const WebKit::DictionaryPopupInfo&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class ExecuteSavedCommandBySelector {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ExecuteSavedCommandBySelector"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    explicit ExecuteSavedCommandBySelector(const String& selector)
        : m_arguments(selector)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class RegisterWebProcessAccessibilityToken {
public:
    typedef std::tuple<IPC::DataReference> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RegisterWebProcessAccessibilityToken"); }
    static const bool isSync = false;

    explicit RegisterWebProcessAccessibilityToken(const IPC::DataReference& data)
        : m_arguments(data)
    {
    }

    const std::tuple<const IPC::DataReference&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const IPC::DataReference&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class PluginFocusOrWindowFocusChanged {
public:
    typedef std::tuple<uint64_t, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("PluginFocusOrWindowFocusChanged"); }
    static const bool isSync = false;

    PluginFocusOrWindowFocusChanged(uint64_t pluginComplexTextInputIdentifier, bool pluginHasFocusAndWindowHasFocus)
        : m_arguments(pluginComplexTextInputIdentifier, pluginHasFocusAndWindowHasFocus)
    {
    }

    const std::tuple<uint64_t, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, bool> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class SetPluginComplexTextInputState {
public:
    typedef std::tuple<uint64_t, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SetPluginComplexTextInputState"); }
    static const bool isSync = false;

    SetPluginComplexTextInputState(uint64_t pluginComplexTextInputIdentifier, uint64_t complexTextInputState)
        : m_arguments(pluginComplexTextInputIdentifier, complexTextInputState)
    {
    }

    const std::tuple<uint64_t, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, uint64_t> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class GetIsSpeaking {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("GetIsSpeaking"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class Speak {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("Speak"); }
    static const bool isSync = false;

    explicit Speak(const String& string)
        : m_arguments(string)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class StopSpeaking {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StopSpeaking"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class MakeFirstResponder {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("MakeFirstResponder"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class SearchWithSpotlight {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SearchWithSpotlight"); }
    static const bool isSync = false;

    explicit SearchWithSpotlight(const String& string)
        : m_arguments(string)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class SearchTheWeb {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SearchTheWeb"); }
    static const bool isSync = false;

    explicit SearchTheWeb(const String& string)
        : m_arguments(string)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if USE(APPKIT)
class SubstitutionsPanelIsShowing {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SubstitutionsPanelIsShowing"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(MAC)
class ShowCorrectionPanel {
public:
    typedef std::tuple<int32_t, WebCore::FloatRect, String, String, Vector<String>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowCorrectionPanel"); }
    static const bool isSync = false;

    ShowCorrectionPanel(int32_t panelType, const WebCore::FloatRect& boundingBoxOfReplacedString, const String& replacedString, const String& replacementString, const Vector<String>& alternativeReplacementStrings)
        : m_arguments(panelType, boundingBoxOfReplacedString, replacedString, replacementString, alternativeReplacementStrings)
    {
    }

    const std::tuple<int32_t, const WebCore::FloatRect&, const String&, const String&, const Vector<String>&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t, const WebCore::FloatRect&, const String&, const String&, const Vector<String>&> m_arguments;
};
#endif

#if PLATFORM(MAC)
class DismissCorrectionPanel {
public:
    typedef std::tuple<int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DismissCorrectionPanel"); }
    static const bool isSync = false;

    explicit DismissCorrectionPanel(int32_t reason)
        : m_arguments(reason)
    {
    }

    const std::tuple<int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t> m_arguments;
};
#endif

#if PLATFORM(MAC)
class DismissCorrectionPanelSoon {
public:
    typedef std::tuple<int32_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DismissCorrectionPanelSoon"); }
    static const bool isSync = true;

    typedef IPC::Arguments<String&> Reply;
    explicit DismissCorrectionPanelSoon(int32_t reason)
        : m_arguments(reason)
    {
    }

    const std::tuple<int32_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t> m_arguments;
};
#endif

#if PLATFORM(MAC)
class RecordAutocorrectionResponse {
public:
    typedef std::tuple<int32_t, String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RecordAutocorrectionResponse"); }
    static const bool isSync = false;

    RecordAutocorrectionResponse(int32_t responseType, const String& replacedString, const String& replacementString)
        : m_arguments(responseType, replacedString, replacementString)
    {
    }

    const std::tuple<int32_t, const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<int32_t, const String&, const String&> m_arguments;
};
#endif

#if USE(DICTATION_ALTERNATIVES)
class ShowDictationAlternativeUI {
public:
    typedef std::tuple<WebCore::FloatRect, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowDictationAlternativeUI"); }
    static const bool isSync = false;

    ShowDictationAlternativeUI(const WebCore::FloatRect& boundingBoxOfDictatedText, uint64_t dictationContext)
        : m_arguments(boundingBoxOfDictatedText, dictationContext)
    {
    }

    const std::tuple<const WebCore::FloatRect&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatRect&, uint64_t> m_arguments;
};
#endif

#if USE(DICTATION_ALTERNATIVES)
class RemoveDictationAlternatives {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RemoveDictationAlternatives"); }
    static const bool isSync = false;

    explicit RemoveDictationAlternatives(uint64_t dictationContext)
        : m_arguments(dictationContext)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};
#endif

#if USE(DICTATION_ALTERNATIVES)
class DictationAlternatives {
public:
    typedef std::tuple<uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DictationAlternatives"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<String>&> Reply;
    explicit DictationAlternatives(uint64_t dictationContext)
        : m_arguments(dictationContext)
    {
    }

    const std::tuple<uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t> m_arguments;
};
#endif

#if PLUGIN_ARCHITECTURE(X11)
class CreatePluginContainer {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("CreatePluginContainer"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint64_t&> Reply;
    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLUGIN_ARCHITECTURE(X11)
class WindowedPluginGeometryDidChange {
public:
    typedef std::tuple<WebCore::IntRect, WebCore::IntRect, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WindowedPluginGeometryDidChange"); }
    static const bool isSync = false;

    WindowedPluginGeometryDidChange(const WebCore::IntRect& frameRect, const WebCore::IntRect& clipRect, uint64_t windowID)
        : m_arguments(frameRect, clipRect, windowID)
    {
    }

    const std::tuple<const WebCore::IntRect&, const WebCore::IntRect&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::IntRect&, const WebCore::IntRect&, uint64_t> m_arguments;
};
#endif

#if PLUGIN_ARCHITECTURE(X11)
class WindowedPluginVisibilityDidChange {
public:
    typedef std::tuple<bool, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WindowedPluginVisibilityDidChange"); }
    static const bool isSync = false;

    WindowedPluginVisibilityDidChange(bool isVisible, uint64_t windowID)
        : m_arguments(isVisible, windowID)
    {
    }

    const std::tuple<bool, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<bool, uint64_t> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DynamicViewportUpdateChangedTarget {
public:
    typedef std::tuple<double, WebCore::FloatPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DynamicViewportUpdateChangedTarget"); }
    static const bool isSync = false;

    DynamicViewportUpdateChangedTarget(double newTargetScale, const WebCore::FloatPoint& newScrollPosition)
        : m_arguments(newTargetScale, newScrollPosition)
    {
    }

    const std::tuple<double, const WebCore::FloatPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<double, const WebCore::FloatPoint&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class RestorePageState {
public:
    typedef std::tuple<WebCore::FloatRect, double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RestorePageState"); }
    static const bool isSync = false;

    RestorePageState(const WebCore::FloatRect& exposedRect, double scale)
        : m_arguments(exposedRect, scale)
    {
    }

    const std::tuple<const WebCore::FloatRect&, double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatRect&, double> m_arguments;
};
#endif

#if PLATFORM(IOS)
class RestorePageCenterAndScale {
public:
    typedef std::tuple<WebCore::FloatPoint, double> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("RestorePageCenterAndScale"); }
    static const bool isSync = false;

    RestorePageCenterAndScale(const WebCore::FloatPoint& unobscuredCenter, double scale)
        : m_arguments(unobscuredCenter, scale)
    {
    }

    const std::tuple<const WebCore::FloatPoint&, double>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::FloatPoint&, double> m_arguments;
};
#endif

#if PLATFORM(IOS)
class DidGetTapHighlightGeometries {
public:
    typedef std::tuple<uint64_t, WebCore::Color, Vector<WebCore::FloatQuad>, WebCore::IntSize, WebCore::IntSize, WebCore::IntSize, WebCore::IntSize> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidGetTapHighlightGeometries"); }
    static const bool isSync = false;

    DidGetTapHighlightGeometries(uint64_t requestID, const WebCore::Color& color, const Vector<WebCore::FloatQuad>& geometries, const WebCore::IntSize& topLeftRadius, const WebCore::IntSize& topRightRadius, const WebCore::IntSize& bottomLeftRadius, const WebCore::IntSize& bottomRightRadius)
        : m_arguments(requestID, color, geometries, topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius)
    {
    }

    const std::tuple<uint64_t, const WebCore::Color&, const Vector<WebCore::FloatQuad>&, const WebCore::IntSize&, const WebCore::IntSize&, const WebCore::IntSize&, const WebCore::IntSize&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<uint64_t, const WebCore::Color&, const Vector<WebCore::FloatQuad>&, const WebCore::IntSize&, const WebCore::IntSize&, const WebCore::IntSize&, const WebCore::IntSize&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class StartAssistingNode {
public:
    typedef std::tuple<WebKit::AssistedNodeInformation, bool, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StartAssistingNode"); }
    static const bool isSync = false;

    StartAssistingNode(const WebKit::AssistedNodeInformation& information, bool userIsInteracting, bool blurPreviousNode, const WebKit::InjectedBundleUserMessageEncoder& userData)
        : m_arguments(information, userIsInteracting, blurPreviousNode, userData)
    {
    }

    const std::tuple<const WebKit::AssistedNodeInformation&, bool, bool, const WebKit::InjectedBundleUserMessageEncoder&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::AssistedNodeInformation&, bool, bool, const WebKit::InjectedBundleUserMessageEncoder&> m_arguments;
};
#endif

#if PLATFORM(IOS)
class StopAssistingNode {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("StopAssistingNode"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if PLATFORM(IOS)
class NotifyRevealedSelection {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("NotifyRevealedSelection"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class ShowInspectorHighlight {
public:
    typedef std::tuple<WebCore::Highlight> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowInspectorHighlight"); }
    static const bool isSync = false;

    explicit ShowInspectorHighlight(const WebCore::Highlight& highlight)
        : m_arguments(highlight)
    {
    }

    const std::tuple<const WebCore::Highlight&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::Highlight&> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class HideInspectorHighlight {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("HideInspectorHighlight"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class ShowInspectorIndication {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowInspectorIndication"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class HideInspectorIndication {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("HideInspectorIndication"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class EnableInspectorNodeSearch {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("EnableInspectorNodeSearch"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
class DisableInspectorNodeSearch {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DisableInspectorNodeSearch"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};
#endif

class SaveRecentSearches {
public:
    typedef std::tuple<String, Vector<String>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SaveRecentSearches"); }
    static const bool isSync = false;

    SaveRecentSearches(const String& name, const Vector<String>& searchItems)
        : m_arguments(name, searchItems)
    {
    }

    const std::tuple<const String&, const Vector<String>&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const Vector<String>&> m_arguments;
};

class LoadRecentSearches {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("LoadRecentSearches"); }
    static const bool isSync = true;

    typedef IPC::Arguments<Vector<String>&> Reply;
    explicit LoadRecentSearches(const String& name)
        : m_arguments(name)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};

#if (USE(SOUP) && !ENABLE(CUSTOM_PROTOCOLS))
class DidReceiveURIRequest {
public:
    typedef std::tuple<String, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidReceiveURIRequest"); }
    static const bool isSync = false;

    DidReceiveURIRequest(const String& uriString, uint64_t requestID)
        : m_arguments(uriString, requestID)
    {
    }

    const std::tuple<const String&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint64_t> m_arguments;
};
#endif

class SavePDFToFileInDownloadsFolder {
public:
    typedef std::tuple<String, String, IPC::DataReference> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SavePDFToFileInDownloadsFolder"); }
    static const bool isSync = false;

    SavePDFToFileInDownloadsFolder(const String& suggestedFilename, const String& originatingURLString, const IPC::DataReference& data)
        : m_arguments(suggestedFilename, originatingURLString, data)
    {
    }

    const std::tuple<const String&, const String&, const IPC::DataReference&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const IPC::DataReference&> m_arguments;
};

#if PLATFORM(COCOA)
class SavePDFToTemporaryFolderAndOpenWithNativeApplication {
public:
    typedef std::tuple<String, String, IPC::DataReference, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("SavePDFToTemporaryFolderAndOpenWithNativeApplication"); }
    static const bool isSync = false;

    SavePDFToTemporaryFolderAndOpenWithNativeApplication(const String& suggestedFilename, const String& originatingURLString, const IPC::DataReference& data, const String& pdfUUID)
        : m_arguments(suggestedFilename, originatingURLString, data, pdfUUID)
    {
    }

    const std::tuple<const String&, const String&, const IPC::DataReference&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&, const IPC::DataReference&, const String&> m_arguments;
};
#endif

#if PLATFORM(COCOA)
class OpenPDFFromTemporaryFolderWithNativeApplication {
public:
    typedef std::tuple<String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("OpenPDFFromTemporaryFolderWithNativeApplication"); }
    static const bool isSync = false;

    explicit OpenPDFFromTemporaryFolderWithNativeApplication(const String& pdfUUID)
        : m_arguments(pdfUUID)
    {
    }

    const std::tuple<const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&> m_arguments;
};
#endif

#if ENABLE(NETSCAPE_PLUGIN_API)
class FindPlugin {
public:
    typedef std::tuple<String, uint32_t, String, String, String, bool> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("FindPlugin"); }
    static const bool isSync = true;

    typedef IPC::Arguments<uint64_t&, String&, uint32_t&, String&> Reply;
    FindPlugin(const String& mimeType, uint32_t processType, const String& urlString, const String& frameURLString, const String& pageURLString, bool allowOnlyApplicationPlugins)
        : m_arguments(mimeType, processType, urlString, frameURLString, pageURLString, allowOnlyApplicationPlugins)
    {
    }

    const std::tuple<const String&, uint32_t, const String&, const String&, const String&, bool>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, uint32_t, const String&, const String&, const String&, bool> m_arguments;
};
#endif

class DidUpdateViewState {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidUpdateViewState"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

class DidSaveToPageCache {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidSaveToPageCache"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

#if ENABLE(SUBTLE_CRYPTO)
class WrapCryptoKey {
public:
    typedef std::tuple<Vector<uint8_t>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WrapCryptoKey"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&, Vector<uint8_t>&> Reply;
    explicit WrapCryptoKey(const Vector<uint8_t>& key)
        : m_arguments(key)
    {
    }

    const std::tuple<const Vector<uint8_t>&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const Vector<uint8_t>&> m_arguments;
};
#endif

#if ENABLE(SUBTLE_CRYPTO)
class UnwrapCryptoKey {
public:
    typedef std::tuple<Vector<uint8_t>> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("UnwrapCryptoKey"); }
    static const bool isSync = true;

    typedef IPC::Arguments<bool&, Vector<uint8_t>&> Reply;
    explicit UnwrapCryptoKey(const Vector<uint8_t>& wrappedKey)
        : m_arguments(wrappedKey)
    {
    }

    const std::tuple<const Vector<uint8_t>&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const Vector<uint8_t>&> m_arguments;
};
#endif

#if (ENABLE(TELEPHONE_NUMBER_DETECTION) && PLATFORM(MAC))
class ShowTelephoneNumberMenu {
public:
    typedef std::tuple<String, WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowTelephoneNumberMenu"); }
    static const bool isSync = false;

    ShowTelephoneNumberMenu(const String& telephoneNumber, const WebCore::IntPoint& point)
        : m_arguments(telephoneNumber, point)
    {
    }

    const std::tuple<const String&, const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const WebCore::IntPoint&> m_arguments;
};
#endif

#if ENABLE(SERVICE_CONTROLS)
class ShowSelectionServiceMenu {
public:
    typedef std::tuple<IPC::DataReference, Vector<String>, bool, WebCore::IntPoint> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ShowSelectionServiceMenu"); }
    static const bool isSync = false;

    ShowSelectionServiceMenu(const IPC::DataReference& selectionAsRTFD, const Vector<String>& telephoneNumbers, bool isEditable, const WebCore::IntPoint& point)
        : m_arguments(selectionAsRTFD, telephoneNumbers, isEditable, point)
    {
    }

    const std::tuple<const IPC::DataReference&, const Vector<String>&, bool, const WebCore::IntPoint&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const IPC::DataReference&, const Vector<String>&, bool, const WebCore::IntPoint&> m_arguments;
};
#endif

#if USE(QUICK_LOOK)
class DidStartLoadForQuickLookDocumentInMainFrame {
public:
    typedef std::tuple<String, String> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidStartLoadForQuickLookDocumentInMainFrame"); }
    static const bool isSync = false;

    DidStartLoadForQuickLookDocumentInMainFrame(const String& fileName, const String& uti)
        : m_arguments(fileName, uti)
    {
    }

    const std::tuple<const String&, const String&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const String&, const String&> m_arguments;
};
#endif

#if USE(QUICK_LOOK)
class DidFinishLoadForQuickLookDocumentInMainFrame {
public:
    typedef std::tuple<WebKit::QuickLookDocumentData> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("DidFinishLoadForQuickLookDocumentInMainFrame"); }
    static const bool isSync = false;

    explicit DidFinishLoadForQuickLookDocumentInMainFrame(const WebKit::QuickLookDocumentData& data)
        : m_arguments(data)
    {
    }

    const std::tuple<const WebKit::QuickLookDocumentData&>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebKit::QuickLookDocumentData&> m_arguments;
};
#endif

class WillChangeCurrentHistoryItemForMainFrame {
public:
    typedef std::tuple<> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("WillChangeCurrentHistoryItemForMainFrame"); }
    static const bool isSync = false;

    const std::tuple<>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<> m_arguments;
};

#if ENABLE(CONTENT_FILTERING)
class ContentFilterDidBlockLoadForFrame {
public:
    typedef std::tuple<WebCore::ContentFilter, uint64_t> DecodeType;

    static IPC::StringReference receiverName() { return messageReceiverName(); }
    static IPC::StringReference name() { return IPC::StringReference("ContentFilterDidBlockLoadForFrame"); }
    static const bool isSync = false;

    ContentFilterDidBlockLoadForFrame(const WebCore::ContentFilter& contentFilter, uint64_t frameID)
        : m_arguments(contentFilter, frameID)
    {
    }

    const std::tuple<const WebCore::ContentFilter&, uint64_t>& arguments() const
    {
        return m_arguments;
    }

private:
    std::tuple<const WebCore::ContentFilter&, uint64_t> m_arguments;
};
#endif

} // namespace WebPageProxy
} // namespace Messages

#endif // WebPageProxyMessages_h
