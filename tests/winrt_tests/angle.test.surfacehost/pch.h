//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
#pragma once

#include "targetver.h"

#include "CppUnitTest.h"
#include <Windows.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <windows.applicationmodel.core.h>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

#include <EGL\egl.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Microsoft::WRL;

#include "MockCoreWindow.h"
#include "GenericInterfaces.h"
#include "surfacehost.h"
#include "winrt\corewindowhost.h"
#include "winrt\winrtutils.h"

