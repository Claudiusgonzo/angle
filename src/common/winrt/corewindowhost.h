//
// Copyright (c) 2002-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// corewindowhost.h: Host for managing ICoreWindow native window types.

#ifndef COMMON_COREWINDOWHOST_H_
#define COMMON_COREWINDOWHOST_H_

#include "common/winrt/iinspectablehost.h"

typedef ABI::Windows::Foundation::__FITypedEventHandler_2_Windows__CUI__CCore__CCoreWindow_Windows__CUI__CCore__CWindowSizeChangedEventArgs_t IWindowSizeChangedEventHandler;

class CoreWindowHost : public IInspectableSurfaceHost, public std::enable_shared_from_this<CoreWindowHost>
{
public:
    ~CoreWindowHost();
    bool initialize(EGLNativeWindowType window);
    bool registerForSizeChangeEvents();
    void unregisterForSizeChangeEvents();
    HRESULT createSwapChain(ID3D11Device* device, DXGIFactory* factory, DXGI_FORMAT format, unsigned int width, unsigned int height, DXGISwapChain** swapChain);
private:
    ComPtr<ABI::Windows::UI::Core::ICoreWindow> mCoreWindow;
};

[uuid(7F924F66-EBAE-40E5-A10B-B8F35E245190)]
class CoreWindowSizeChangedHandler :
    public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ABI::Windows::Foundation::__FITypedEventHandler_2_Windows__CUI__CCore__CCoreWindow_Windows__CUI__CCore__CWindowSizeChangedEventArgs_t>
{
public:
    CoreWindowSizeChangedHandler() { }
    HRESULT RuntimeClassInitialize(std::shared_ptr<IInspectableSurfaceHost> host)
    {
        if (!host)
        {
            return E_INVALIDARG;
        }

        mHost = host;
        return S_OK;
    }

    // IWindowSizeChangedEventHandler
    IFACEMETHOD(Invoke)(ABI::Windows::UI::Core::ICoreWindow *sender, ABI::Windows::UI::Core::IWindowSizeChangedEventArgs *e)
    {
        std::shared_ptr<IInspectableSurfaceHost> host = mHost.lock();
        if (host)
        {
            ABI::Windows::Foundation::Size windowSize;
            if (SUCCEEDED(e->get_Size(&windowSize)))
            {
                SIZE windowSizeInPixels = { (long)winrt::convertDipsToPixels(windowSize.Width), (long)winrt::convertDipsToPixels(windowSize.Height) };
                host->setNewClientSize(windowSizeInPixels);
            }
        }

        return S_OK;
    }

private:
    std::weak_ptr<IInspectableSurfaceHost> mHost;
};

HRESULT getCoreWindowSizeInPixels(ComPtr<ABI::Windows::UI::Core::ICoreWindow> coreWindow, RECT* windowSize);

#endif // COMMON_COREWINDOWHOST_H_
