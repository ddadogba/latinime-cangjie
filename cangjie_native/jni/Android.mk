# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# LOCAL_ARM_MODE := arm

LOCAL_MODULE    := chinese_table
LOCAL_SRC_FILES := chinese_table.c input_method.c quick_method.c cangjie_method.c stroke_method.c phrase.c

# LOCAL_LDLIBS    := -lm -llog -landroid
LOCAL_LDLIBS    := -lm -llog 

include $(BUILD_SHARED_LIBRARY)
