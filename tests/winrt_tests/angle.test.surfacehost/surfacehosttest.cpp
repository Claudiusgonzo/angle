//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace angle_test_surfacehost
{
    TEST_CLASS(SurfaceHostTests)
    {
    public:
        TEST_METHOD(CreateCoreWindowSurfaceHostFailsWithNullNativeWindowType)
        {
            rx::SurfaceHost host(nullptr);
            Assert::IsFalse(host.initialize());
        }
        TEST_METHOD(CreateCoreWindowSurfaceHostSucceedsWithValidNativeWindowType)
        {
            ComPtr<ABI::Windows::UI::Core::ICoreWindow> coreWindow = Make<MockCoreWindow>();
            rx::SurfaceHost host(coreWindow.Get());
            Assert::IsTrue(host.initialize());
        }
        TEST_METHOD(CreateCoreWindowSurfaceHostFailsWithInvalidIInspectableNativeWindowType)
        {
            ComPtr<IAmInspectable> notCoreWindow = Make<GenericIInspectable>();
            rx::SurfaceHost host(notCoreWindow.Get());
            Assert::IsFalse(host.initialize());
        }
        TEST_METHOD(CreateCoreWindowSurfaceHostFailsWithInvalidIUnknownNativeWindowType)
        {
            ComPtr<IAmUnknown> notIInspectable = Make<GenericIUnknown>();
            rx::SurfaceHost host((IInspectable*)notIInspectable.Get());
            Assert::IsFalse(host.initialize());
            
        }
        TEST_METHOD(SizeChangedEventIsunregisteredOnDestruction)
        {
            ComPtr<MockCoreWindow> coreWindow = Make<MockCoreWindow>();
            {
                rx::SurfaceHost host(coreWindow.Get());
                Assert::IsTrue(host.initialize());
                Assert::AreEqual(1, coreWindow->GetSizeChangeRegistrationCount());
            }
            Assert::AreEqual(0, coreWindow->GetSizeChangeRegistrationCount());
        }
        TEST_METHOD(CurrentBoundsIsQueriedOnInitialization)
        {
            ComPtr<MockCoreWindow> coreWindow = Make<MockCoreWindow>();
            ABI::Windows::Foundation::Rect expectedBounds = { 0, 0, 1024, 768 };
            coreWindow->SetExpectedBounds(expectedBounds);

            rx::SurfaceHost host(coreWindow.Get());
            Assert::IsTrue(host.initialize());
            Assert::AreEqual(1, coreWindow->GetBoundsQueryCount());

            RECT clientRect = { 0, 0, 0, 0 };
            RECT expectedRect = { 0, 0, 1024, 768 };
            Assert::IsTrue(host.getClientRect(&clientRect));
            Assert::IsTrue(clientRect == expectedRect);

            Assert::AreEqual(1, coreWindow->GetBoundsQueryCount());
        }
        TEST_METHOD(IsIconicAlwaysReturnsFalse)
        {
            ComPtr<ABI::Windows::UI::Core::ICoreWindow> coreWindow = Make<MockCoreWindow>();
            rx::SurfaceHost host(coreWindow.Get());
            Assert::IsTrue(host.initialize());
            Assert::IsFalse(host.isIconic());
        }
        TEST_METHOD(SizeChangedEventSignalUpdatesClientRectToExpectedValue)
        {
            // Create core window with 800x600 bounds
            ComPtr<MockCoreWindow> coreWindow = Make<MockCoreWindow>();
            ABI::Windows::Foundation::Rect expectedBounds = { 0, 0, 800, 600 };
            coreWindow->SetExpectedBounds(expectedBounds);

            // Create surface, expecting to see 800x600 bounds
            rx::SurfaceHost host(coreWindow.Get());
            Assert::IsTrue(host.initialize());

            RECT clientRect = { 0, 0, 0, 0 };
            RECT expectedRect = { 0, 0, 800, 600 };
            Assert::IsTrue(host.getClientRect(&clientRect));
            Assert::IsTrue(clientRect == expectedRect);

            // Update core window to 1024x768 bounds, and cause a sizeChangedEvent to be signaled
            clientRect = { 0, 0, 0, 0 };
            expectedBounds = { 0, 0, 1024, 768 };
            coreWindow->UpdateSizeAndSignalChangedEvent(expectedBounds);

            expectedRect = { 0, 0, 1024, 768 };
            Assert::IsTrue(host.getClientRect(&clientRect));
            Assert::IsTrue(clientRect == expectedRect);
        }
    };
}