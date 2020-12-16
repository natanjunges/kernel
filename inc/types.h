/* Copyright 2020 Natan Junges

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#define null (void *)0

#define STATUS_OK 0
#define STATUS_ILLEGAL_ARGUMENT 1 //Some argument has unexpected value
#define STATUS_NULL_POINTER_ARGUMENT 2 //Some pointer argument is null
#define STATUS_ARGUMENT_OUT_OF_BOUNDS 3 //Some argument is out of its bounds
#define STATUS_ILLEGAL_VALUE 4 //A value was unexpected during execution
#define STATUS_NULL_POINTER_VALUE 5 //A pointer was null during execution
#define STATUS_VALUE_OUT_OF_BOUNDS 6 //A value was out of its bounds during execution
#define STATUS_EMPTY_THING 7 //An empty thing was unexpectedly found
#define STATUS_NO_SUCH_THING 8 //A finding procedure failed
#define STATUS_DUPLICATE_THING 9 //A duplicate thing was unexpectedly found
#define STATUS_TOO_MANY_THINGS 10 //More things were found than expected

#define STATUS uint8_t

#define VALUE_STATUS(name) struct VALUE_STATUS_ ## name

#define DEFINE_VALUE_STATUS(name, type) \
VALUE_STATUS(name) {\
    type value;\
    STATUS status;\
} __attribute__((packed))
