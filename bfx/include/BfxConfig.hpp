#pragma once

// Components to be added to the main application in the framework
#define BFX_APP_COMPONENT_USB_HOST             (1)

// The maximum number of child objects that can be registered with the application
#define BFX_MAX_CHILD_OBJECTS               (10U)

// The maximim number of sinks that can be registered with the logger
#define BFX_MAX_LOG_SINKS                   (3U)

// The maximum number of HID devices that can be supported by the USB host
#define BFX_MAX_USB_HID_DEVICES             (2U)

// The size of the shared memory region used by the BFX framework for commuincations
// between the cores
#define BFX_SHARED_MEMORY_SIZE              (128U * 1024U) // 128 KB

// The strategy for the allocator for the m33 ns/m55 shared memory
#define BFX_USE_O1HEAP_ALLOCATOR                   (1U)
#define BFX_USE_SIMPLE_ALLOCATOR                   (2U)
#define BFX_SHARED_MEMORY_ALLOCATOR_STRATEGY       (BFX_USE_O1HEAP_ALLOCATOR)
