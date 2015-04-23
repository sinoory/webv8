/*
 * Copyright (C) 2013, 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PageConsoleClient.h"

#include "Chrome.h"
#include "ChromeClient.h"
#include "Document.h"
#include "Frame.h"
//#include "InspectorConsoleInstrumentation.h"
#include "InspectorController.h"
//#include "JSMainThreadExecState.h"
#include "MainFrame.h"
#include "Page.h"
#include "ScriptableDocumentParser.h"
#include "Settings.h"
//#include <bindings/ScriptValue.h>
#include <inspector/ScriptArguments.h>
#include <inspector/ScriptCallStack.h>
#include <inspector/ScriptCallStackFactory.h>

using namespace Inspector;

namespace WebCore {

PageConsoleClient::PageConsoleClient(Page& page)
    :Console(&(page.mainFrame())), m_page(page)
{
}

PageConsoleClient::~PageConsoleClient()
{
}

static int muteCount = 0;
static bool printExceptions = false;

bool PageConsoleClient::shouldPrintExceptions()
{
    return printExceptions;
}

void PageConsoleClient::setShouldPrintExceptions(bool print)
{
    printExceptions = print;
}


void PageConsoleClient::addMessage(MessageSource source, MessageLevel level, String& message, PassRefPtr<WebCore::ScriptCallStack> callStack){
    //addMessage(source,LogMessageType,level,message,"undefinedurl",0,0,callStack); //CMP_ERROR_UNCLEAR implement it
}

void PageConsoleClient::addMessage(MessageSource source, MessageLevel level, const String& message){
    //addMessage(source,LogMessageType,level,message,"undefinedurl",0,0);
}

void PageConsoleClient::addMessage(MessageSource source, MessageLevel level, const String& message, const String& sourceURL,unsigned line,unsigned col){
    //addMessage(source,LogMessageType,level,message,line,sourceURL);
}
void PageConsoleClient::addMessage(MessageSource source, MessageLevel level, const String& message, const String& url, unsigned lineNumber, unsigned columnNumber, PassRefPtr<ScriptCallStack> callStack)
{
    //addMessage(source,LogMessageType,level,message,line,sourceURL,callStack);
}
void PageConsoleClient::addMessage(MessageSource source, MessageLevel level, const String& message, unsigned long requestIdentifier, Document* document)
{
    String url;
    if (document)
        url = document->url().string();


    unsigned line = 0;
    unsigned column = 0;
    if (document && document->parsing() && !document->isInDocumentWrite() && document->scriptableDocumentParser()) {
        ScriptableDocumentParser* parser = document->scriptableDocumentParser();
        if (!parser->isWaitingForScripts() /*&& !JSMainThreadExecState::currentState()*/) {
            TextPosition position = parser->textPosition();
            line = position.m_line.oneBasedInt();
            column = position.m_column.oneBasedInt();
        }
    }
    //addMessage(source, LogMessageType,level, message,  line, column, url);
}

void PageConsoleClient::mute()
{
    muteCount++;
}

void PageConsoleClient::unmute()
{
    ASSERT(muteCount > 0);
    muteCount--;
}



void PageConsoleClient::clearProfiles()
{
    //m_profiles.clear();
}

} // namespace WebCore
