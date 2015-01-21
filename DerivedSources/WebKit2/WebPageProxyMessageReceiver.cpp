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

#include "config.h"

#include "WebPageProxy.h"

#include "ArgumentCoders.h"
#if PLATFORM(IOS)
#include "AssistedNodeInformation.h"
#endif
#if PLATFORM(MAC) || PLATFORM(COCOA)
#include "AttributedString.h"
#endif
#if ENABLE(CONTEXT_MENUS)
#include "ContextMenuContextData.h"
#endif
#include "DataReference.h"
#if PLATFORM(COCOA)
#include "DictionaryPopupInfo.h"
#endif
#include "EditingRange.h"
#include "EditorState.h"
#include "HandleMessage.h"
#include "InjectedBundleUserMessageCoders.h"
#if PLATFORM(IOS)
#include "InteractionInformationAtPosition.h"
#endif
#include "MessageDecoder.h"
#include "NavigationActionData.h"
#include "PlatformPopupMenuData.h"
#if USE(QUICK_LOOK)
#include "QuickLookDocumentData.h"
#endif
#include "SandboxExtension.h"
#include "ShareableBitmap.h"
#if PLATFORM(COCOA) && ENABLE(DRAG_SUPPORT) || PLATFORM(IOS)
#include "SharedMemory.h"
#endif
#if ENABLE(CONTEXT_MENUS)
#include "WebContextMenuItemData.h"
#endif
#include "WebCoreArgumentCoders.h"
#include "WebHitTestResult.h"
#include "WebPageCreationParameters.h"
#include "WebPageProxyMessages.h"
#include "WebPopupItem.h"
#include <WebCore/AuthenticationChallenge.h>
#include <WebCore/CertificateInfo.h>
#include <WebCore/Color.h>
#if ENABLE(CONTENT_FILTERING)
#include <WebCore/ContentFilter.h>
#endif
#if !PLATFORM(IOS)
#include <WebCore/Cursor.h>
#endif
#if PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
#include <WebCore/DragData.h>
#endif
#include <WebCore/FileChooser.h>
#if PLATFORM(IOS)
#include <WebCore/FloatPoint.h>
#endif
#if PLATFORM(IOS)
#include <WebCore/FloatQuad.h>
#endif
#include <WebCore/FloatRect.h>
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
#include <WebCore/InspectorOverlay.h>
#endif
#include <WebCore/IntPoint.h>
#include <WebCore/IntRect.h>
#if PLATFORM(EFL) || PLATFORM(IOS)
#include <WebCore/IntSize.h>
#endif
#include <WebCore/ProtectionSpace.h>
#include <WebCore/ResourceError.h>
#include <WebCore/ResourceRequest.h>
#include <WebCore/ResourceResponse.h>
#include <WebCore/TextCheckerClient.h>
#include <WebCore/TextChecking.h>
#include <WebCore/ViewportArguments.h>
#include <WebCore/WindowFeatures.h>
#include <utility>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace Messages {

namespace WebPageProxy {

AddMessageToConsole::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

AddMessageToConsole::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool AddMessageToConsole::DelayedReply::send()
{
    ASSERT(m_encoder);
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

RunJavaScriptAlert::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

RunJavaScriptAlert::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool RunJavaScriptAlert::DelayedReply::send()
{
    ASSERT(m_encoder);
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

RunJavaScriptConfirm::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

RunJavaScriptConfirm::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool RunJavaScriptConfirm::DelayedReply::send(bool result)
{
    ASSERT(m_encoder);
    *m_encoder << result;
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

RunJavaScriptPrompt::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

RunJavaScriptPrompt::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool RunJavaScriptPrompt::DelayedReply::send(const String& result)
{
    ASSERT(m_encoder);
    *m_encoder << result;
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

ExceededDatabaseQuota::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

ExceededDatabaseQuota::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool ExceededDatabaseQuota::DelayedReply::send(uint64_t newQuota)
{
    ASSERT(m_encoder);
    *m_encoder << newQuota;
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

ReachedApplicationCacheOriginQuota::DelayedReply::DelayedReply(PassRefPtr<IPC::Connection> connection, std::unique_ptr<IPC::MessageEncoder> encoder)
    : m_connection(connection)
    , m_encoder(WTF::move(encoder))
{
}

ReachedApplicationCacheOriginQuota::DelayedReply::~DelayedReply()
{
    ASSERT(!m_connection);
}

bool ReachedApplicationCacheOriginQuota::DelayedReply::send(uint64_t newQuota)
{
    ASSERT(m_encoder);
    *m_encoder << newQuota;
    bool _result = m_connection->sendSyncReply(WTF::move(m_encoder));
    m_connection = nullptr;
    return _result;
}

} // namespace WebPageProxy

} // namespace Messages

namespace WebKit {

void WebPageProxy::didReceiveMessage(IPC::Connection* connection, IPC::MessageDecoder& decoder)
{
    if (decoder.messageName() == Messages::WebPageProxy::ShowPage::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowPage>(decoder, this, &WebPageProxy::showPage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ClosePage::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ClosePage>(decoder, this, &WebPageProxy::closePage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::MouseDidMoveOverElement::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::MouseDidMoveOverElement>(decoder, this, &WebPageProxy::mouseDidMoveOverElement);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidBeginTrackingPotentialLongMousePress::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidBeginTrackingPotentialLongMousePress>(decoder, this, &WebPageProxy::didBeginTrackingPotentialLongMousePress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRecognizeLongMousePress::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidRecognizeLongMousePress>(decoder, this, &WebPageProxy::didRecognizeLongMousePress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCancelTrackingPotentialLongMousePress::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidCancelTrackingPotentialLongMousePress>(decoder, this, &WebPageProxy::didCancelTrackingPotentialLongMousePress);
        return;
    }
#if ENABLE(NETSCAPE_PLUGIN_API)
    if (decoder.messageName() == Messages::WebPageProxy::UnavailablePluginButtonClicked::name()) {
        IPC::handleMessage<Messages::WebPageProxy::UnavailablePluginButtonClicked>(decoder, this, &WebPageProxy::unavailablePluginButtonClicked);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeViewportProperties::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangeViewportProperties>(decoder, this, &WebPageProxy::didChangeViewportProperties);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveEvent::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidReceiveEvent>(decoder, this, &WebPageProxy::didReceiveEvent);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::StopResponsivenessTimer::name()) {
        IPC::handleMessage<Messages::WebPageProxy::StopResponsivenessTimer>(decoder, this, &WebPageProxy::stopResponsivenessTimer);
        return;
    }
#if !PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::SetCursor::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetCursor>(decoder, this, &WebPageProxy::setCursor);
        return;
    }
#endif
#if !PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::SetCursorHiddenUntilMouseMoves::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetCursorHiddenUntilMouseMoves>(decoder, this, &WebPageProxy::setCursorHiddenUntilMouseMoves);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SetStatusText::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetStatusText>(decoder, this, &WebPageProxy::setStatusText);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetToolTip::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetToolTip>(decoder, this, &WebPageProxy::setToolTip);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetFocus::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetFocus>(decoder, this, &WebPageProxy::setFocus);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::TakeFocus::name()) {
        IPC::handleMessage<Messages::WebPageProxy::TakeFocus>(decoder, this, &WebPageProxy::takeFocus);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::FocusedFrameChanged::name()) {
        IPC::handleMessage<Messages::WebPageProxy::FocusedFrameChanged>(decoder, this, &WebPageProxy::focusedFrameChanged);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::FrameSetLargestFrameChanged::name()) {
        IPC::handleMessage<Messages::WebPageProxy::FrameSetLargestFrameChanged>(decoder, this, &WebPageProxy::frameSetLargestFrameChanged);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetRenderTreeSize::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetRenderTreeSize>(decoder, this, &WebPageProxy::setRenderTreeSize);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetToolbarsAreVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetToolbarsAreVisible>(decoder, this, &WebPageProxy::setToolbarsAreVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetMenuBarIsVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetMenuBarIsVisible>(decoder, this, &WebPageProxy::setMenuBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetStatusBarIsVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetStatusBarIsVisible>(decoder, this, &WebPageProxy::setStatusBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetIsResizable::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetIsResizable>(decoder, this, &WebPageProxy::setIsResizable);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetWindowFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetWindowFrame>(decoder, this, &WebPageProxy::setWindowFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PageDidScroll::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageDidScroll>(decoder, this, &WebPageProxy::pageDidScroll);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunOpenPanel::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RunOpenPanel>(decoder, this, &WebPageProxy::runOpenPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunModal::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RunModal>(decoder, this, &WebPageProxy::runModal);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::NotifyScrollerThumbIsVisibleInRect::name()) {
        IPC::handleMessage<Messages::WebPageProxy::NotifyScrollerThumbIsVisibleInRect>(decoder, this, &WebPageProxy::notifyScrollerThumbIsVisibleInRect);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RecommendedScrollbarStyleDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RecommendedScrollbarStyleDidChange>(decoder, this, &WebPageProxy::recommendedScrollbarStyleDidChange);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeScrollbarsForMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangeScrollbarsForMainFrame>(decoder, this, &WebPageProxy::didChangeScrollbarsForMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeScrollOffsetPinningForMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangeScrollOffsetPinningForMainFrame>(decoder, this, &WebPageProxy::didChangeScrollOffsetPinningForMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangePageCount::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangePageCount>(decoder, this, &WebPageProxy::didChangePageCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PageExtendedBackgroundColorDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageExtendedBackgroundColorDidChange>(decoder, this, &WebPageProxy::pageExtendedBackgroundColorDidChange);
        return;
    }
#if ENABLE(NETSCAPE_PLUGIN_API)
    if (decoder.messageName() == Messages::WebPageProxy::DidFailToInitializePlugin::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFailToInitializePlugin>(decoder, this, &WebPageProxy::didFailToInitializePlugin);
        return;
    }
#endif
#if ENABLE(NETSCAPE_PLUGIN_API)
    if (decoder.messageName() == Messages::WebPageProxy::DidBlockInsecurePluginVersion::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidBlockInsecurePluginVersion>(decoder, this, &WebPageProxy::didBlockInsecurePluginVersion);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SetCanShortCircuitHorizontalWheelEvents::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetCanShortCircuitHorizontalWheelEvents>(decoder, this, &WebPageProxy::setCanShortCircuitHorizontalWheelEvents);
        return;
    }
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPageProxy::PageDidRequestScroll::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageDidRequestScroll>(decoder, this, &WebPageProxy::pageDidRequestScroll);
        return;
    }
#endif
#if USE(TILED_BACKING_STORE)
    if (decoder.messageName() == Messages::WebPageProxy::PageTransitionViewportReady::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageTransitionViewportReady>(decoder, this, &WebPageProxy::pageTransitionViewportReady);
        return;
    }
#endif
#if USE(COORDINATED_GRAPHICS)
    if (decoder.messageName() == Messages::WebPageProxy::DidFindZoomableArea::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFindZoomableArea>(decoder, this, &WebPageProxy::didFindZoomableArea);
        return;
    }
#endif
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeContentSize::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangeContentSize>(decoder, this, &WebPageProxy::didChangeContentSize);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::ShowColorPicker::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowColorPicker>(decoder, this, &WebPageProxy::showColorPicker);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::SetColorPickerColor::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetColorPickerColor>(decoder, this, &WebPageProxy::setColorPickerColor);
        return;
    }
#endif
#if ENABLE(INPUT_TYPE_COLOR)
    if (decoder.messageName() == Messages::WebPageProxy::EndColorPicker::name()) {
        IPC::handleMessage<Messages::WebPageProxy::EndColorPicker>(decoder, this, &WebPageProxy::endColorPicker);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForNewWindowAction::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForNewWindowAction>(decoder, this, &WebPageProxy::decidePolicyForNewWindowAction);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UnableToImplementPolicy::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::UnableToImplementPolicy>(decoder, this, &WebPageProxy::unableToImplementPolicy);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidChangeProgress::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidChangeProgress>(decoder, this, &WebPageProxy::didChangeProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishProgress::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFinishProgress>(decoder, this, &WebPageProxy::didFinishProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidStartProgress::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidStartProgress>(decoder, this, &WebPageProxy::didStartProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetNetworkRequestsInProgress::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetNetworkRequestsInProgress>(decoder, this, &WebPageProxy::setNetworkRequestsInProgress);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCreateMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidCreateMainFrame>(decoder, this, &WebPageProxy::didCreateMainFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCreateSubframe::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidCreateSubframe>(decoder, this, &WebPageProxy::didCreateSubframe);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidStartProvisionalLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidStartProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didStartProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveServerRedirectForProvisionalLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidReceiveServerRedirectForProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didReceiveServerRedirectForProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailProvisionalLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFailProvisionalLoadForFrame>(decoder, this, &WebPageProxy::didFailProvisionalLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCommitLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidCommitLoadForFrame>(decoder, this, &WebPageProxy::didCommitLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFailLoadForFrame>(decoder, this, &WebPageProxy::didFailLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishDocumentLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFinishDocumentLoadForFrame>(decoder, this, &WebPageProxy::didFinishDocumentLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFinishLoadForFrame>(decoder, this, &WebPageProxy::didFinishLoadForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFirstLayoutForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFirstLayoutForFrame>(decoder, this, &WebPageProxy::didFirstLayoutForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFirstVisuallyNonEmptyLayoutForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidFirstVisuallyNonEmptyLayoutForFrame>(decoder, this, &WebPageProxy::didFirstVisuallyNonEmptyLayoutForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidLayout::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidLayout>(decoder, this, &WebPageProxy::didLayout);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRemoveFrameFromHierarchy::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidRemoveFrameFromHierarchy>(decoder, this, &WebPageProxy::didRemoveFrameFromHierarchy);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveTitleForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidReceiveTitleForFrame>(decoder, this, &WebPageProxy::didReceiveTitleForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDisplayInsecureContentForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidDisplayInsecureContentForFrame>(decoder, this, &WebPageProxy::didDisplayInsecureContentForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidRunInsecureContentForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidRunInsecureContentForFrame>(decoder, this, &WebPageProxy::didRunInsecureContentForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDetectXSSForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidDetectXSSForFrame>(decoder, this, &WebPageProxy::didDetectXSSForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidSameDocumentNavigationForFrame::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DidSameDocumentNavigationForFrame>(decoder, this, &WebPageProxy::didSameDocumentNavigationForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDestroyNavigation::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidDestroyNavigation>(decoder, this, &WebPageProxy::didDestroyNavigation);
        return;
    }

    if (decoder.messageName() == Messages::WebPageProxy::FrameDidBecomeFrameSet::name()) {
        IPC::handleMessage<Messages::WebPageProxy::FrameDidBecomeFrameSet>(decoder, this, &WebPageProxy::frameDidBecomeFrameSet);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadingDataForCustomContentProvider::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFinishLoadingDataForCustomContentProvider>(decoder, this, &WebPageProxy::didFinishLoadingDataForCustomContentProvider);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::WillSubmitForm::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::WillSubmitForm>(decoder, this, &WebPageProxy::willSubmitForm);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::VoidCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::VoidCallback>(decoder, this, &WebPageProxy::voidCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DataCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DataCallback>(decoder, this, &WebPageProxy::dataCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ImageCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ImageCallback>(decoder, this, &WebPageProxy::imageCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::StringCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::StringCallback>(decoder, this, &WebPageProxy::stringCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ScriptValueCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ScriptValueCallback>(decoder, this, &WebPageProxy::scriptValueCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ComputedPagesCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ComputedPagesCallback>(decoder, this, &WebPageProxy::computedPagesCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ValidateCommandCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ValidateCommandCallback>(decoder, this, &WebPageProxy::validateCommandCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::EditingRangeCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::EditingRangeCallback>(decoder, this, &WebPageProxy::editingRangeCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UnsignedCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::UnsignedCallback>(decoder, this, &WebPageProxy::unsignedCallback);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RectForCharacterRangeCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RectForCharacterRangeCallback>(decoder, this, &WebPageProxy::rectForCharacterRangeCallback);
        return;
    }
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::AttributedStringForCharacterRangeCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::AttributedStringForCharacterRangeCallback>(decoder, this, &WebPageProxy::attributedStringForCharacterRangeCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::GestureCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GestureCallback>(decoder, this, &WebPageProxy::gestureCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::TouchesCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::TouchesCallback>(decoder, this, &WebPageProxy::touchesCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::AutocorrectionDataCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::AutocorrectionDataCallback>(decoder, this, &WebPageProxy::autocorrectionDataCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::AutocorrectionContextCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::AutocorrectionContextCallback>(decoder, this, &WebPageProxy::autocorrectionContextCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DictationContextCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DictationContextCallback>(decoder, this, &WebPageProxy::dictationContextCallback);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DidReceivePositionInformation::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidReceivePositionInformation>(decoder, this, &WebPageProxy::didReceivePositionInformation);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::SaveImageToLibrary::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SaveImageToLibrary>(decoder, this, &WebPageProxy::saveImageToLibrary);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DidUpdateBlockSelectionWithTouch::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidUpdateBlockSelectionWithTouch>(decoder, this, &WebPageProxy::didUpdateBlockSelectionWithTouch);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::ShowPlaybackTargetPicker::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowPlaybackTargetPicker>(decoder, this, &WebPageProxy::showPlaybackTargetPicker);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::ZoomToRect::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ZoomToRect>(decoder, this, &WebPageProxy::zoomToRect);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::CommitPotentialTapFailed::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CommitPotentialTapFailed>(decoder, this, &WebPageProxy::commitPotentialTapFailed);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DidNotHandleTapAsClick::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidNotHandleTapAsClick>(decoder, this, &WebPageProxy::didNotHandleTapAsClick);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::ViewportMetaTagWidthDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ViewportMetaTagWidthDidChange>(decoder, this, &WebPageProxy::viewportMetaTagWidthDidChange);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::SetUsesMinimalUI::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetUsesMinimalUI>(decoder, this, &WebPageProxy::setUsesMinimalUI);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishDrawingPagesToPDF::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFinishDrawingPagesToPDF>(decoder, this, &WebPageProxy::didFinishDrawingPagesToPDF);
        return;
    }
#endif
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::PrintFinishedCallback::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PrintFinishedCallback>(decoder, this, &WebPageProxy::printFinishedCallback);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::PageScaleFactorDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageScaleFactorDidChange>(decoder, this, &WebPageProxy::pageScaleFactorDidChange);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PageZoomFactorDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PageZoomFactorDidChange>(decoder, this, &WebPageProxy::pageZoomFactorDidChange);
        return;
    }
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::BindAccessibilityTree::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BindAccessibilityTree>(decoder, this, &WebPageProxy::bindAccessibilityTree);
        return;
    }
#endif
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::SetInputMethodState::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetInputMethodState>(decoder, this, &WebPageProxy::setInputMethodState);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardAddItem::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardAddItem>(decoder, this, &WebPageProxy::backForwardAddItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardClear::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardClear>(decoder, this, &WebPageProxy::backForwardClear);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::WillGoToBackForwardListItem::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::WillGoToBackForwardListItem>(decoder, this, &WebPageProxy::willGoToBackForwardListItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RegisterEditCommandForUndo::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RegisterEditCommandForUndo>(decoder, this, &WebPageProxy::registerEditCommandForUndo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ClearAllEditCommands::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ClearAllEditCommands>(decoder, this, &WebPageProxy::clearAllEditCommands);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RegisterInsertionUndoGrouping::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RegisterInsertionUndoGrouping>(decoder, this, &WebPageProxy::registerInsertionUndoGrouping);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::EditorStateChanged::name()) {
        IPC::handleMessage<Messages::WebPageProxy::EditorStateChanged>(decoder, this, &WebPageProxy::editorStateChanged);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CompositionWasCanceled::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CompositionWasCanceled>(decoder, this, &WebPageProxy::compositionWasCanceled);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidCountStringMatches::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidCountStringMatches>(decoder, this, &WebPageProxy::didCountStringMatches);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SetFindIndicator::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetFindIndicator>(decoder, this, &WebPageProxy::setFindIndicator);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFindString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFindString>(decoder, this, &WebPageProxy::didFindString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFailToFindString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFailToFindString>(decoder, this, &WebPageProxy::didFailToFindString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidFindStringMatches::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFindStringMatches>(decoder, this, &WebPageProxy::didFindStringMatches);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidGetImageForFindMatch::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidGetImageForFindMatch>(decoder, this, &WebPageProxy::didGetImageForFindMatch);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShowPopupMenu::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowPopupMenu>(decoder, this, &WebPageProxy::showPopupMenu);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::HidePopupMenu::name()) {
        IPC::handleMessage<Messages::WebPageProxy::HidePopupMenu>(decoder, this, &WebPageProxy::hidePopupMenu);
        return;
    }
#if ENABLE(CONTEXT_MENUS)
    if (decoder.messageName() == Messages::WebPageProxy::ShowContextMenu::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::ShowContextMenu>(decoder, this, &WebPageProxy::showContextMenu);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveAuthenticationChallenge::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidReceiveAuthenticationChallenge>(decoder, this, &WebPageProxy::didReceiveAuthenticationChallenge);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RequestGeolocationPermissionForFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RequestGeolocationPermissionForFrame>(decoder, this, &WebPageProxy::requestGeolocationPermissionForFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RequestNotificationPermission::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RequestNotificationPermission>(decoder, this, &WebPageProxy::requestNotificationPermission);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShowNotification::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowNotification>(decoder, this, &WebPageProxy::showNotification);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CancelNotification::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CancelNotification>(decoder, this, &WebPageProxy::cancelNotification);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ClearNotifications::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ClearNotifications>(decoder, this, &WebPageProxy::clearNotifications);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidDestroyNotification::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidDestroyNotification>(decoder, this, &WebPageProxy::didDestroyNotification);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UpdateSpellingUIWithMisspelledWord::name()) {
        IPC::handleMessage<Messages::WebPageProxy::UpdateSpellingUIWithMisspelledWord>(decoder, this, &WebPageProxy::updateSpellingUIWithMisspelledWord);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::UpdateSpellingUIWithGrammarString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::UpdateSpellingUIWithGrammarString>(decoder, this, &WebPageProxy::updateSpellingUIWithGrammarString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::LearnWord::name()) {
        IPC::handleMessage<Messages::WebPageProxy::LearnWord>(decoder, this, &WebPageProxy::learnWord);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::IgnoreWord::name()) {
        IPC::handleMessage<Messages::WebPageProxy::IgnoreWord>(decoder, this, &WebPageProxy::ignoreWord);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RequestCheckingOfString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RequestCheckingOfString>(decoder, this, &WebPageProxy::requestCheckingOfString);
        return;
    }
#if ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::DidPerformDragControllerAction::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidPerformDragControllerAction>(decoder, this, &WebPageProxy::didPerformDragControllerAction);
        return;
    }
#endif
#if PLATFORM(COCOA) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::SetDragImage::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetDragImage>(decoder, this, &WebPageProxy::setDragImage);
        return;
    }
#endif
#if PLATFORM(COCOA) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::SetPromisedData::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetPromisedData>(decoder, this, &WebPageProxy::setPromisedData);
        return;
    }
#endif
#if PLATFORM(GTK) && ENABLE(DRAG_SUPPORT)
    if (decoder.messageName() == Messages::WebPageProxy::StartDrag::name()) {
        IPC::handleMessage<Messages::WebPageProxy::StartDrag>(decoder, this, &WebPageProxy::startDrag);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::DidPerformDictionaryLookup::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidPerformDictionaryLookup>(decoder, this, &WebPageProxy::didPerformDictionaryLookup);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::RegisterWebProcessAccessibilityToken::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RegisterWebProcessAccessibilityToken>(decoder, this, &WebPageProxy::registerWebProcessAccessibilityToken);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::PluginFocusOrWindowFocusChanged::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PluginFocusOrWindowFocusChanged>(decoder, this, &WebPageProxy::pluginFocusOrWindowFocusChanged);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::SetPluginComplexTextInputState::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SetPluginComplexTextInputState>(decoder, this, &WebPageProxy::setPluginComplexTextInputState);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::Speak::name()) {
        IPC::handleMessage<Messages::WebPageProxy::Speak>(decoder, this, &WebPageProxy::speak);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::StopSpeaking::name()) {
        IPC::handleMessage<Messages::WebPageProxy::StopSpeaking>(decoder, this, &WebPageProxy::stopSpeaking);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::MakeFirstResponder::name()) {
        IPC::handleMessage<Messages::WebPageProxy::MakeFirstResponder>(decoder, this, &WebPageProxy::makeFirstResponder);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::SearchWithSpotlight::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SearchWithSpotlight>(decoder, this, &WebPageProxy::searchWithSpotlight);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::SearchTheWeb::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SearchTheWeb>(decoder, this, &WebPageProxy::searchTheWeb);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::ShowCorrectionPanel::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowCorrectionPanel>(decoder, this, &WebPageProxy::showCorrectionPanel);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::DismissCorrectionPanel::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DismissCorrectionPanel>(decoder, this, &WebPageProxy::dismissCorrectionPanel);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::RecordAutocorrectionResponse::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RecordAutocorrectionResponse>(decoder, this, &WebPageProxy::recordAutocorrectionResponse);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::ShowDictationAlternativeUI::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowDictationAlternativeUI>(decoder, this, &WebPageProxy::showDictationAlternativeUI);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::RemoveDictationAlternatives::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RemoveDictationAlternatives>(decoder, this, &WebPageProxy::removeDictationAlternatives);
        return;
    }
#endif
#if PLUGIN_ARCHITECTURE(X11)
    if (decoder.messageName() == Messages::WebPageProxy::WindowedPluginGeometryDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::WindowedPluginGeometryDidChange>(decoder, this, &WebPageProxy::windowedPluginGeometryDidChange);
        return;
    }
#endif
#if PLUGIN_ARCHITECTURE(X11)
    if (decoder.messageName() == Messages::WebPageProxy::WindowedPluginVisibilityDidChange::name()) {
        IPC::handleMessage<Messages::WebPageProxy::WindowedPluginVisibilityDidChange>(decoder, this, &WebPageProxy::windowedPluginVisibilityDidChange);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DynamicViewportUpdateChangedTarget::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DynamicViewportUpdateChangedTarget>(decoder, this, &WebPageProxy::dynamicViewportUpdateChangedTarget);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::RestorePageState::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RestorePageState>(decoder, this, &WebPageProxy::restorePageState);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::RestorePageCenterAndScale::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RestorePageCenterAndScale>(decoder, this, &WebPageProxy::restorePageCenterAndScale);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::DidGetTapHighlightGeometries::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidGetTapHighlightGeometries>(decoder, this, &WebPageProxy::didGetTapHighlightGeometries);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::StartAssistingNode::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::StartAssistingNode>(decoder, this, &WebPageProxy::startAssistingNode);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::StopAssistingNode::name()) {
        IPC::handleMessage<Messages::WebPageProxy::StopAssistingNode>(decoder, this, &WebPageProxy::stopAssistingNode);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::NotifyRevealedSelection::name()) {
        IPC::handleMessage<Messages::WebPageProxy::NotifyRevealedSelection>(decoder, this, &WebPageProxy::notifyRevealedSelection);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::ShowInspectorHighlight::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowInspectorHighlight>(decoder, this, &WebPageProxy::showInspectorHighlight);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::HideInspectorHighlight::name()) {
        IPC::handleMessage<Messages::WebPageProxy::HideInspectorHighlight>(decoder, this, &WebPageProxy::hideInspectorHighlight);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::ShowInspectorIndication::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowInspectorIndication>(decoder, this, &WebPageProxy::showInspectorIndication);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::HideInspectorIndication::name()) {
        IPC::handleMessage<Messages::WebPageProxy::HideInspectorIndication>(decoder, this, &WebPageProxy::hideInspectorIndication);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::EnableInspectorNodeSearch::name()) {
        IPC::handleMessage<Messages::WebPageProxy::EnableInspectorNodeSearch>(decoder, this, &WebPageProxy::enableInspectorNodeSearch);
        return;
    }
#endif
#if (PLATFORM(IOS) && ENABLE(INSPECTOR))
    if (decoder.messageName() == Messages::WebPageProxy::DisableInspectorNodeSearch::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DisableInspectorNodeSearch>(decoder, this, &WebPageProxy::disableInspectorNodeSearch);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SaveRecentSearches::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SaveRecentSearches>(decoder, this, &WebPageProxy::saveRecentSearches);
        return;
    }
#if (USE(SOUP) && !ENABLE(CUSTOM_PROTOCOLS))
    if (decoder.messageName() == Messages::WebPageProxy::DidReceiveURIRequest::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidReceiveURIRequest>(decoder, this, &WebPageProxy::didReceiveURIRequest);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::SavePDFToFileInDownloadsFolder::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SavePDFToFileInDownloadsFolder>(decoder, this, &WebPageProxy::savePDFToFileInDownloadsFolder);
        return;
    }
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::SavePDFToTemporaryFolderAndOpenWithNativeApplication::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SavePDFToTemporaryFolderAndOpenWithNativeApplication>(decoder, this, &WebPageProxy::savePDFToTemporaryFolderAndOpenWithNativeApplication);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::OpenPDFFromTemporaryFolderWithNativeApplication::name()) {
        IPC::handleMessage<Messages::WebPageProxy::OpenPDFFromTemporaryFolderWithNativeApplication>(decoder, this, &WebPageProxy::openPDFFromTemporaryFolderWithNativeApplication);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DidUpdateViewState::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidUpdateViewState>(decoder, this, &WebPageProxy::didUpdateViewState);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DidSaveToPageCache::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidSaveToPageCache>(decoder, this, &WebPageProxy::didSaveToPageCache);
        return;
    }
#if (ENABLE(TELEPHONE_NUMBER_DETECTION) && PLATFORM(MAC))
    if (decoder.messageName() == Messages::WebPageProxy::ShowTelephoneNumberMenu::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowTelephoneNumberMenu>(decoder, this, &WebPageProxy::showTelephoneNumberMenu);
        return;
    }
#endif
#if ENABLE(SERVICE_CONTROLS)
    if (decoder.messageName() == Messages::WebPageProxy::ShowSelectionServiceMenu::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShowSelectionServiceMenu>(decoder, this, &WebPageProxy::showSelectionServiceMenu);
        return;
    }
#endif
#if USE(QUICK_LOOK)
    if (decoder.messageName() == Messages::WebPageProxy::DidStartLoadForQuickLookDocumentInMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidStartLoadForQuickLookDocumentInMainFrame>(decoder, this, &WebPageProxy::didStartLoadForQuickLookDocumentInMainFrame);
        return;
    }
#endif
#if USE(QUICK_LOOK)
    if (decoder.messageName() == Messages::WebPageProxy::DidFinishLoadForQuickLookDocumentInMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DidFinishLoadForQuickLookDocumentInMainFrame>(decoder, this, &WebPageProxy::didFinishLoadForQuickLookDocumentInMainFrame);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::WillChangeCurrentHistoryItemForMainFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::WillChangeCurrentHistoryItemForMainFrame>(decoder, this, &WebPageProxy::willChangeCurrentHistoryItemForMainFrame);
        return;
    }
#if ENABLE(CONTENT_FILTERING)
    if (decoder.messageName() == Messages::WebPageProxy::ContentFilterDidBlockLoadForFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ContentFilterDidBlockLoadForFrame>(decoder, this, &WebPageProxy::contentFilterDidBlockLoadForFrame);
        return;
    }
#endif
    UNUSED_PARAM(connection);
    UNUSED_PARAM(decoder);
    ASSERT_NOT_REACHED();
}

void WebPageProxy::didReceiveSyncMessage(IPC::Connection* connection, IPC::MessageDecoder& decoder, std::unique_ptr<IPC::MessageEncoder>& replyEncoder)
{
    if (decoder.messageName() == Messages::WebPageProxy::CreateNewPage::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CreateNewPage>(decoder, *replyEncoder, this, &WebPageProxy::createNewPage);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::AddMessageToConsole::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::AddMessageToConsole>(connection, decoder, replyEncoder, this, &WebPageProxy::addMessageToConsole);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptAlert::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::RunJavaScriptAlert>(connection, decoder, replyEncoder, this, &WebPageProxy::runJavaScriptAlert);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptConfirm::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::RunJavaScriptConfirm>(connection, decoder, replyEncoder, this, &WebPageProxy::runJavaScriptConfirm);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RunJavaScriptPrompt::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::RunJavaScriptPrompt>(connection, decoder, replyEncoder, this, &WebPageProxy::runJavaScriptPrompt);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ShouldInterruptJavaScript::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ShouldInterruptJavaScript>(decoder, *replyEncoder, this, &WebPageProxy::shouldInterruptJavaScript);
        return;
    }
#if ENABLE(WEBGL)
    if (decoder.messageName() == Messages::WebPageProxy::WebGLPolicyForURL::name()) {
        IPC::handleMessage<Messages::WebPageProxy::WebGLPolicyForURL>(decoder, *replyEncoder, this, &WebPageProxy::webGLPolicyForURL);
        return;
    }
#endif
#if ENABLE(WEBGL)
    if (decoder.messageName() == Messages::WebPageProxy::ResolveWebGLPolicyForURL::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ResolveWebGLPolicyForURL>(decoder, *replyEncoder, this, &WebPageProxy::resolveWebGLPolicyForURL);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::GetToolbarsAreVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetToolbarsAreVisible>(decoder, *replyEncoder, this, &WebPageProxy::getToolbarsAreVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetMenuBarIsVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetMenuBarIsVisible>(decoder, *replyEncoder, this, &WebPageProxy::getMenuBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetStatusBarIsVisible::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetStatusBarIsVisible>(decoder, *replyEncoder, this, &WebPageProxy::getStatusBarIsVisible);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetIsResizable::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetIsResizable>(decoder, *replyEncoder, this, &WebPageProxy::getIsResizable);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetWindowFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetWindowFrame>(decoder, *replyEncoder, this, &WebPageProxy::getWindowFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ScreenToRootView::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ScreenToRootView>(decoder, *replyEncoder, this, &WebPageProxy::screenToRootView);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::RootViewToScreen::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RootViewToScreen>(decoder, *replyEncoder, this, &WebPageProxy::rootViewToScreen);
        return;
    }
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::AccessibilityScreenToRootView::name()) {
        IPC::handleMessage<Messages::WebPageProxy::AccessibilityScreenToRootView>(decoder, *replyEncoder, this, &WebPageProxy::accessibilityScreenToRootView);
        return;
    }
#endif
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::RootViewToAccessibilityScreen::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RootViewToAccessibilityScreen>(decoder, *replyEncoder, this, &WebPageProxy::rootViewToAccessibilityScreen);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::RunBeforeUnloadConfirmPanel::name()) {
        IPC::handleMessage<Messages::WebPageProxy::RunBeforeUnloadConfirmPanel>(decoder, *replyEncoder, this, &WebPageProxy::runBeforeUnloadConfirmPanel);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::PrintFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::PrintFrame>(decoder, *replyEncoder, this, &WebPageProxy::printFrame);
        return;
    }
#if PLATFORM(EFL)
    if (decoder.messageName() == Messages::WebPageProxy::HandleInputMethodKeydown::name()) {
        IPC::handleMessage<Messages::WebPageProxy::HandleInputMethodKeydown>(decoder, *replyEncoder, this, &WebPageProxy::handleInputMethodKeydown);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForResponseSync::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForResponseSync>(decoder, *replyEncoder, this, &WebPageProxy::decidePolicyForResponseSync);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::DecidePolicyForNavigationAction::name()) {
        IPC::handleMessageVariadic<Messages::WebPageProxy::DecidePolicyForNavigationAction>(decoder, *replyEncoder, this, &WebPageProxy::decidePolicyForNavigationAction);
        return;
    }
#if PLATFORM(IOS)
    if (decoder.messageName() == Messages::WebPageProxy::InterpretKeyEvent::name()) {
        IPC::handleMessage<Messages::WebPageProxy::InterpretKeyEvent>(decoder, *replyEncoder, this, &WebPageProxy::interpretKeyEvent);
        return;
    }
#endif
#if PLATFORM(GTK)
    if (decoder.messageName() == Messages::WebPageProxy::GetEditorCommandsForKeyEvent::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetEditorCommandsForKeyEvent>(decoder, *replyEncoder, this, &WebPageProxy::getEditorCommandsForKeyEvent);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardGoToItem::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardGoToItem>(decoder, *replyEncoder, this, &WebPageProxy::backForwardGoToItem);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardItemAtIndex::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardItemAtIndex>(decoder, *replyEncoder, this, &WebPageProxy::backForwardItemAtIndex);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardBackListCount::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardBackListCount>(decoder, *replyEncoder, this, &WebPageProxy::backForwardBackListCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::BackForwardForwardListCount::name()) {
        IPC::handleMessage<Messages::WebPageProxy::BackForwardForwardListCount>(decoder, *replyEncoder, this, &WebPageProxy::backForwardForwardListCount);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CanUndoRedo::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CanUndoRedo>(decoder, *replyEncoder, this, &WebPageProxy::canUndoRedo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ExecuteUndoRedo::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ExecuteUndoRedo>(decoder, *replyEncoder, this, &WebPageProxy::executeUndoRedo);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CanAuthenticateAgainstProtectionSpaceInFrame::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CanAuthenticateAgainstProtectionSpaceInFrame>(decoder, *replyEncoder, this, &WebPageProxy::canAuthenticateAgainstProtectionSpaceInFrame);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ExceededDatabaseQuota::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::ExceededDatabaseQuota>(connection, decoder, replyEncoder, this, &WebPageProxy::exceededDatabaseQuota);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::ReachedApplicationCacheOriginQuota::name()) {
        IPC::handleMessageDelayed<Messages::WebPageProxy::ReachedApplicationCacheOriginQuota>(connection, decoder, replyEncoder, this, &WebPageProxy::reachedApplicationCacheOriginQuota);
        return;
    }
#if USE(UNIFIED_TEXT_CHECKING)
    if (decoder.messageName() == Messages::WebPageProxy::CheckTextOfParagraph::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CheckTextOfParagraph>(decoder, *replyEncoder, this, &WebPageProxy::checkTextOfParagraph);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::CheckSpellingOfString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CheckSpellingOfString>(decoder, *replyEncoder, this, &WebPageProxy::checkSpellingOfString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::CheckGrammarOfString::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CheckGrammarOfString>(decoder, *replyEncoder, this, &WebPageProxy::checkGrammarOfString);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::SpellingUIIsShowing::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SpellingUIIsShowing>(decoder, *replyEncoder, this, &WebPageProxy::spellingUIIsShowing);
        return;
    }
    if (decoder.messageName() == Messages::WebPageProxy::GetGuessesForWord::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetGuessesForWord>(decoder, *replyEncoder, this, &WebPageProxy::getGuessesForWord);
        return;
    }
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::ExecuteSavedCommandBySelector::name()) {
        IPC::handleMessage<Messages::WebPageProxy::ExecuteSavedCommandBySelector>(decoder, *replyEncoder, this, &WebPageProxy::executeSavedCommandBySelector);
        return;
    }
#endif
#if PLATFORM(COCOA)
    if (decoder.messageName() == Messages::WebPageProxy::GetIsSpeaking::name()) {
        IPC::handleMessage<Messages::WebPageProxy::GetIsSpeaking>(decoder, *replyEncoder, this, &WebPageProxy::getIsSpeaking);
        return;
    }
#endif
#if USE(APPKIT)
    if (decoder.messageName() == Messages::WebPageProxy::SubstitutionsPanelIsShowing::name()) {
        IPC::handleMessage<Messages::WebPageProxy::SubstitutionsPanelIsShowing>(decoder, *replyEncoder, this, &WebPageProxy::substitutionsPanelIsShowing);
        return;
    }
#endif
#if PLATFORM(MAC)
    if (decoder.messageName() == Messages::WebPageProxy::DismissCorrectionPanelSoon::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DismissCorrectionPanelSoon>(decoder, *replyEncoder, this, &WebPageProxy::dismissCorrectionPanelSoon);
        return;
    }
#endif
#if USE(DICTATION_ALTERNATIVES)
    if (decoder.messageName() == Messages::WebPageProxy::DictationAlternatives::name()) {
        IPC::handleMessage<Messages::WebPageProxy::DictationAlternatives>(decoder, *replyEncoder, this, &WebPageProxy::dictationAlternatives);
        return;
    }
#endif
#if PLUGIN_ARCHITECTURE(X11)
    if (decoder.messageName() == Messages::WebPageProxy::CreatePluginContainer::name()) {
        IPC::handleMessage<Messages::WebPageProxy::CreatePluginContainer>(decoder, *replyEncoder, this, &WebPageProxy::createPluginContainer);
        return;
    }
#endif
    if (decoder.messageName() == Messages::WebPageProxy::LoadRecentSearches::name()) {
        IPC::handleMessage<Messages::WebPageProxy::LoadRecentSearches>(decoder, *replyEncoder, this, &WebPageProxy::loadRecentSearches);
        return;
    }
#if ENABLE(NETSCAPE_PLUGIN_API)
    if (decoder.messageName() == Messages::WebPageProxy::FindPlugin::name()) {
        IPC::handleMessage<Messages::WebPageProxy::FindPlugin>(decoder, *replyEncoder, this, &WebPageProxy::findPlugin);
        return;
    }
#endif
#if ENABLE(SUBTLE_CRYPTO)
    if (decoder.messageName() == Messages::WebPageProxy::WrapCryptoKey::name()) {
        IPC::handleMessage<Messages::WebPageProxy::WrapCryptoKey>(decoder, *replyEncoder, this, &WebPageProxy::wrapCryptoKey);
        return;
    }
#endif
#if ENABLE(SUBTLE_CRYPTO)
    if (decoder.messageName() == Messages::WebPageProxy::UnwrapCryptoKey::name()) {
        IPC::handleMessage<Messages::WebPageProxy::UnwrapCryptoKey>(decoder, *replyEncoder, this, &WebPageProxy::unwrapCryptoKey);
        return;
    }
#endif
    UNUSED_PARAM(connection);
    UNUSED_PARAM(decoder);
    UNUSED_PARAM(replyEncoder);
    ASSERT_NOT_REACHED();
}

} // namespace WebKit
