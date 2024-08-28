# A GUI for nodebackend

## License 

Nodefrontend is BSD licensed but contains LGPL components.
The understanding is that linking to these components is allowed as long as users of the library are furnished with their source code and rights to reverse engineer for debugging purposes.

## Release schedule

This library is developed in my spare time for fun.  As such, there is no release achedule except to say when I deem it ready.  Its primary purpose is to serve as the GUI for another side project so I am not overly concerned by timing of a release.

## Collaboration

Pull requests will be allowed at some point. There will be a coding style and probably automated linting which could be a barrier to contribution.

## Dependencies 

* C++17 compiler (gcc 9.4+, clang16+, Visual Studio 2017+)
* Lua-5.4.x for the declarative format
* gtest for automated testing
* gmock to allow mocking in tests
* benchmark for microbenchmarking
* CMake 3.16+ for the build system
* VulkanSceneGraph for integration demos
* freetype for text rasterisation
* pango for text layout including underlining
* freeglut for OpenGL text demo
* md4c for markdown parsing
* svgpp for SVG parsing
* nodebase for basic features such as Lua support, configuration and streams.
* math library with 2D, 3D vectors and 4x4 matrices
  * required to do translation and rotation in one matrix multiplication
  * required to create an Orthographic projection
* Packages for Linux Debian based
```
sudo apt-get install g++ cmake make freeglut3-dev libfreetype-dev libpango1.0-dev libgtest-dev libgmock-dev libbenchmark-dev liblua5.4-dev git
```

## Build instructions 

### Linux
```
  git clone https://github.com/triblatron/nodefrontend
  mkdir nodefrontend_build && cd nodefrontend_build
  cmake -S ../nodefrontend -B . -DCMAKE_INSTALL_PREFIX=../nodefrontend_install
  make
  make install
  cd ../nodefrontend_install
  source setup.sh
  NodefrontendTest
```
### Windows 
```
git clone https://github.com/triblatron/install --branch x64-win64-vc17 --single-branch install
git clone https://github.com/triblatron/nodefrontend
mkdir nodefrontend_build
cd nodefrontend_build
cmake -S nodefrontend -B . -DDEP_ROOT=../install -DCMAKE_INSTALL_PREFIX=../nodefrontend_install
cmake --build . --target install --config Release
cd nodefrontend_install
setup
NodeFrontendTest
```

## Progress

* Shapes hit test
  * ~~Rounded rectangle~~
  * ~~Circle~~
  * ~~CompositeShape: union of Shape~~
* Widget
  * ~~has-a Shape~~
* ~~Config tree~~
* Window
  * ~~round-tripping of features and status to string and back to flags~~
* Validation
  * ~~Integer types~~

~~Build on Ubuntu 24.04~~

~~Build on Ubuntu 22.04~~

~~Build on Ubuntu 20.04~~

~~Build on Windows 11~~

~~Build on Raspian 12~~


## Next up
* Build on macOS 10.15 Catalina
* ~~Render a full screen quad in a GLUT window~~
* ~~Create a texture of a solid colour~~
* Render a glyph to a texture
* Render a font to a texture
* Render some text using the font texture as a lookup
* Render effects such as underline using pango
  * image that can address individual pixels
  * address windows within the image to allow use of one image for multiple glyphs
    * translate input coordinates by origin of window
  * specify format
    * greyscale
    * rgb
  * copy multiple pixels at once into the image
  * Specify origin convention
    * top left
    * bottom left
* Repeat for Vulkan
* ~~build on Raspberry Pi 5 64bit~~
* build on Raspberry Pi 4 64bit
* draw a window
* design a renderer backend that can consume textured triangles and render to
  * a texture
  * the screen
* implement backend for
  * OpenGL 2.1
  * OpenGL 3.3
  * OpenGL 4.6
  * Vulkan 1.3

## Known working platforms
* Ubuntu 20.04.6 LTS x64 gcc 9
  * Requires buliding Lua from source [triblatron/lua](https://github.com/triblatron/lua)
* Ubuntu 22.04 LTS x64 gcc 11.4.0
* Ubuntu 24.04 LTS x64 gcc 13
* Windows 11 Home 22631.3880 23H2 Visual Studio Community 2022 17.10.0
  * requires [triblatron/install](https://github.com/triblatron/install) branch x64-win64-vc17
* Ubuntu 22.04.4 LTS x64 on WSL gcc 11.4.0
* Raspberry Pi 5 Raspian 12 aarch64 gcc 12.2
* Windows 11 Home 22631.3880 23H2 Visual Studio Community 2019 16.11.39
* Windows 11 Home 22631.3880 23H2 Visual Studio Community 2017 16.9.65

To qualify as working, a platform must:
* provide at least the mininum version of each dependency in its package manager or build from source
* build with no warnings in the nodefrontend code when compiled with -Wall and -Wextra
* pass all the tests
  * unit tests
  * integration tests
  * performance tests
  * manual usability tests

So far, there are only unit tests but that will change in time.

Eventually, there will be automated integration tests that typically assert the presence of particular widgets and calls of event handlers etc.

There will also be automated performance tests that render to texture as a proxy for the screen.  Alongside these will be microbenchmarks that measure performance of various idioms used to implement the library.

Finally there will be manual usability tests that require running the full system and a tester with some UX experience.

## Known failing platforms
none

## Caveats
For some reason no tests are found on Windows unless we have an explicit main().  This is despite not needing one in other projects.  The issue seems to be related to order of linking libraries but is complicated by cmake or visual studio not honouring the order given in the CMakeLists file.
This has been fixed by tweaking the order in the CMakeLists.txt, which now ends up in Visual Studio.
It is necesary to build benchmark from source to make a successful build on VS2017 and VS2019

## Requirements
* User facing rather than quick throw-away debugging for developers
  * Use ImGUI for the latter
* Easy to use API
  * basic tasks should be easy to achieve
  * advanced tasks ahould be possible and have a straightforward set of steps
* Expose widget tree to user of API
  * scene graph with rigid body transforms
  * strict tree
  * rendering backend
    * Use concept of painter?
      * draw primitives such as Shape
    * Generate textured triangles
    * Provide interface to be implemented by integration
    * Agnostic to graphics API
      * OpenGL
      * Vulkan
      * Metal
      * DirectX
    * Needs to be efficient
      * submit batches of triangles rather than one vertex per call like in the days of old
    * Generate text for rendering by integration
      * puts dependencies in integration
        * harfbuzz for text shaping
        * pango to support underlining for keyboard shortcuts and error indication
        * freetype for font metrics and rasterisation
          * Use GL_RED for fonts with greyscale
            * GL_LUMINANCE is deprecated in modern OpenGL
          * Use GL_RGB for fonts with sub-pixel hinting
* Efficiency promoting API
  * Pass by value and allow optimiser to make it fast
  * check whether move constructor can be elided
  * Use std::vector rather than std::list or std::map wherever possible to maximise cache usage.
  * roll our own based on std::vector
  * use stack wrapping a vector
  * use index chasing end for queue
    * keeps memory so not suitable for large queues 
  * use region based allocator to improve locality of reference for std::list
  * use indexing because they are not invalidated by adding elements at the end
  * reserve() elements when we know how many will be added
  * chasing pointers causes cache misses with at least 5x slowdown and can be 100x or more if accessing main memory.
* User-friendly widgets
  * Make it intuitive how to use them
    * A clear small set of interactions
    * consistency so that a particular interaction always has the same effect where available
    * no tutorial required
      * maybe an ideal
    * no internet search required
      * probably won't find anything
  * Avoid gratuitous switching between mouse and keyboard
    * If already using the keyboard, provide a shortcut for the next action in the flow e.g. Ctrl-S to save
    * Support context-specific shortcuts
      * Submit a form (Ctrl-Enter to avoid confusion with a newline in a text area)
      * Cancel a modal dialgoue box 
    * Predictable/consistent behaviour to reduce cognitive load
    * Put keyboard shortcuts in tooltips
  * Provide help on why a widget is disabled/unavailable
    * tooltips even when widget is disabled
      * but not too much information to avoid overwhelming user
    * hover events
* ~~Support config tree for declarative layout of widgets~~
* Support tips
  * context-dependent rather than overly generic
    * align with frequently-performed actions
  * track interaction with tips
    * clicks
    * dismissals
    * refine over time
  * easily dismissable
  * do not interrupt mid-action
    * given after action
  * allow live shortcuts to actions in tips
  * concise, actionable and easy to understand
    * depends on app author
* Support notion of nothing to do
  * no events
  * no redrawing
  * no frames
* Support canvas
  * Grid with snap
  * Zoom
  * Scrollable infinitely
  * Pan
  * Fit to view
    * Set bounds to show all content
    * Orthographic projection matrix?
      * Would need to be 4x4
* Support validation
  * primitive types out of range
  * string type too long or too short
  * prevent invalid input by restricting entry
  * provide only one way to set data that needs validation
    * prevent invalid values from entering the system
    * give feedback on why the input cannot be modified
      * changing would put the value out of range or too long
      * be careful about disabling keys, typically only disallow confirmation or submission of the dialogue but allow change of focus because it might influence the invalid field
* Support dependencies between controls in a dialogue
  * combobox that changes later fields
  * add all combinations and switch between them
  * could cause a cascade of dependencies and resulting explosion of possibilities
  * try to avoid this by deaign
* Support pop-up menu
    * searchable
    * list matches as actions as for pull-down menus
* Support node graph browser
  * navigation forward, backward, up and down
  * drill into complex nodes to any depth
  * show breadcrumb of current path with links to each parent graph
* Support scientific visualisation
  * graphs of signals against time
  * live capture
  * playback
  * multiple plots on one set of axes
  * multiple graphs
  * easy splitting of view
  * select signals
  * same signal can appear on more than one plot
  * units on each axis
  * legend
* Support notion of time provider
  * Allow faking of time to test time-dependent event dispatch 
* Support scheduling of events
* Support concept of space occupied by a widget
  * Reserve space on creation
  * Give back space on deletion
* Support animation of properties
  * position
  * rotation
  * colour
  * alpha
  * spatial occupancy e.g. creation makes room, deletion takes back space
* Support menu bar
  * menus accessible by mouse or keyboard
  * searchable with listing of matches as actions
* Support STL containers in widgets
  * std::list where insertion in middle is common
  * template parameter of container with sensible default
* track cursor position for events
  * can be used to position child windows
* Support child windows as first class citizens
  * dragging happens automatically without having to compute coordinates in event handler
* Support dual-listbox widget
  * available list
  * active list
  * transfer items between lists
  * move items up and down lists
  * use std::list and splicing operations
  * explorer-style selection:
    * Ctrl-click
    * Shift-click
    * Drag a box
  * no indexing
* Support MVC models
  * ideally using POD types without having to inherit framework classes
* Support panes
  * scrollable when required
  * proportinoally sized scroll bars 
* Support markdown in text
  * labels
  * text fields
  * tooltips
* Support different mouse cursor
  * small image
  * property of a widget
* Support mouse enter and leave events
  * start and stop operation
* Support tables
  * configurable alternating background colours
  * tooltips to show full text of cells where it is truncated
  * variable column width by dragging splitters
  * draggable columns
  * custom sorting of data
* Support trees
  * expand node
  * collapse node
  * expand all
  * collapse all
* Support tabbed toolbar
  * avoid design patent
* Support status bar
* Support notifications
  * disappear after configurable period
* Support user settings config
  * Use same format
  * generate settings dialogue
* Support explorer style selection by meta key and click combinations or dragging
  * for all widgets that contain other widgets such as panes
* Support drag and drop modes including none to allow dragging of child windows
  * Allow drag and drop of icons in a palette onto the work area to instantiate an object
  * Allow dragging of child windows without triggering a transfer
* Allow dragging from any point on a child window
* Support prototype pattern by clone()ing widgets from a library into a tree.
  * be careful about which elements are deep copied
* High performance < 1ms render time per frame on all platforms
  * Raspberry Pi 4 1920x1080@60
  * Raspberry Pi 5 1920x1080@120?
  * Windows/Linux/Mac Apple Silicon 4k@120Hz
  * Mac Intel 1920x1080@60Hz
* Testable headless to verify interactions and presence of widgets
  * Use identifiers of widgets rather than positions to make tests robust to layout changes
* Efficient representation to avoid redundant state changes
  * Sort by texture to avoid swapping repeatedly
  * Use texture atlases for fonts and icons to limit number of textures required
  * Sort transparent polygons after opaque polygons to get correct rendering order.
  * Generate whichever is more efficient: indexed triangles or triangle strips.
    * Profile on target hardware, possibly change answer on discrete desktop GPU versus Pi 4.
    * Selectable at runtime by passing different mode token in OpenGL or topology token in Vulkan.
    * Might change based on what is being drawn: text versus widgets
* Support tooltips with markdown
  * Allow Unicode codepoints such as emojis and small images as arbitrary icons.
  * Allow rich content without the overhead of (X)HTML/CSS/Javascript.
* Layout managers with constraint based layout
  * Use well known algorithm suitable for GUIs
* ~~Rounded rectangles~~
* ~~Circles~~
* Bezier curves with handles
  * For signal paths
  * Multi-segment to support complex signal paths
  * quadratic/one handle should be sufficient
  * optional
* Support lines with handles as a simpler alternative to Bezier curves
  * For signal paths
  * Multi-segment to support complex signal paths
  * one handle to change alignment
  * required
* Host widgets at arbitrary points in the widget tree, such as children of rectangles, by allowing different shapes for widgets
* ~~Allow drawing circles outside the bounds of a rectangle to make a composite shape~~
  * should we allow 2D boolean operations?
    * would require a binary tree with operators at the internal nodes and primitives at the leaf nodes
    * can use an external library to solve drawing as long as it generates triangles
* Simple to integrate by rendering textured triangles
  * OpenGL for simplicity
  * Vulkan for performance
* Support push buttons
  * default control highlighting
  * click event handler
* Support checkboxes
  * individual or as items in another control
  * tri-state, typically in a tree to indicate that some portion of an items children are selected
* Support radio buttons
  * individual or as items in another control
  * exclusion groups
* Support disabled controls greying out
  * using colour manipulation or artist input
* Support tabbing between controls
  * Fires onblur as focus leaves a control
  * Fires onfocus as focus enters a control
  * Allow configurable tab order to increase convenience of using keyboard and accessibility
    * Try to avoid dead-ends such as text areas where tab is interpreted as a tab character
    * Provide a meta-key combination to toggle the interpretation of tab by a control between moving focus and control-specific action
    * could use alternative to Tab for tab character in text area
      * simpler
* Support validation of inputs
  * Provide information on why an input is invalid and highlight the input field
    * tooltip, but avoid too much information
  * Provide validators for basic types with ranges
    * std::int_\*, std::uint_\*
    * std::string
    * enums
  * Provide validators for enums
    * Parse and convert to string
    * choices
    * bitmasks
* Support splitters
  * draggable
    * need cursor to show when dragging is allowed
* Support collapsible panes
  * need icon to show state: expanded or collapsed
* Support horizontal and vertical layout
* Support grid layout 
* Support input validation
  * Restrict input to valid characters based on expected type and range
  * give tooltip help on why an input is invalid
* Support keyboard accelerators
  * Modify menu item and label text to include shortcut
  * Detect combination
    * State machine e.g. Ctrl + Shift + C can be done in different orders like Shift + Ctrl + C so it is best thought of as a chord followed by a character rather than a pure sequence
* Simple event system without code generation
* Support high-quality text rendering
* Support Windows 10,11
* Support Linux Ubuntu 20.04, 22.04,24.04
* Support macOS on Intel, Apple Silicon
* Support Raspberry pi 4,5 Raspian 12
* Support reading mouse and keyboard events
  * abstraction of native events
* Support Clean Architecture view model 
* Support data binding with property change events without requiring the GUI to be visible
* Support separation of visual logic from application logic
  * visual logic goes in the code behind
    * should we use nodes to evaluate the visual logic?
      * could connect clicked output on a button to enabled on another control
      * more conplex scenarios would still require code behind so it might be more complexity for not much benefit
* Support declarative creation of widgets using a simple file format
* Focus and blur events
* Support text effects such as bold and underline
  * not as important because we will put the shortcut in a tooltip with a differe
triblatron/nodefrontend
nt font
* Use modern C++ techniques
  * lamdas as callbacks
  * std::thread, std::mutex and std::atomic<>
  * range-based for
  * for-scope local variables
  * std::unique_ptr<> and std::make_unique<>() to automate memory management and avoid leaks, use after free, dangling pointers etc.
    * be wary of performance overhead
* Support actions to allow more than one way to invoke functionality
* Support icons with text in menu items
* Support searchable menus with completion
  * Benchmark trie versus substring for partial matching
    * substring wins by orders of magnitude, but still takes 1100ns to search on a representative set of Blender-style menu paths
* Support tutorial mode
  * Dynamic disabling of widgets to guide user
  * Help notifications
  * Highlighting of widgets
  * Dynamic placement of widgets in flow of text
    * extension of Markdown OR
    * HTML?
* Support rigid body transforms on widgets
* Support integration with VulkanSceneGraph
* No programmer art
  * Need an artist to create visual snippets such as close icon and collapse handle
  * Typically SVG format
  * Pre-render into an atlas at desired resolution
    * need an SVG rasteriser
* Support compound shapes as the bounds of a widget
* Client area and system area on windows
* Component system with composition over inheritance
* Texture atlas for interface elements such as system buttons and handles for collapsible panes
* Property notification system with change events
* Optional system controls and title bar
* Hit detection of shapes
* Support keyboard shortcuts
* Possible boolean operations on shapes
* Activation events
* Inherit visibility from parent
* SVG shapes if parser supports translation to primitives
* No code generation
  * At least C++, could allow Lua instead because it does not require explicit compilation
    * Supported in Lua persistent format as Lua event handlers
    * might need to SWIG the C++ API to make this possible
    * alternative is to have yielding to C++ when a service is required.
      * lightweight but possibly restrictive
* No interference with the CMake configure and generate process.
* One intuitive place to change to add a new property, event, whatever.
* Modular composable components
  * Entity-component system?
  * Widgets as nodes?
* Render only changed areas of the view
* Separate visual style from structure
  * stylesheets?
  * Skinnable
* Designer using its own widgets
  * Generate declarative Lua config
  * self hosting: design the designer using the designer
    * might have head-explosion issues
* allow event loop in any thread
* allow rendering in any thread
  * need an OpenGL context or whatever Vulkan uses
  * can generate triagles in one thread and consume them in another
* Messaging system similar to signal-slots without the MOC or similar
  * put it behind an interface
  * thread safe
  * no blocking of gui thread
  * cannot use futures since they are for sending values to another thread
  * could use coroutines if we bump to C++20 or use Boost or similar
    * another dependency
  * it is unclear how to call a method in another thread safely
    * locks, but how do we make it work for an arbitrary method on an arbitrary object?
* Integration
  * define interface to generate textured triangles
  * define interface to generate text
  * define interface to rasterise SVG
  * they might be the same
  * rasterising text is supported using freetype and pango

## Tips
* SWIG xml mode works only if we %define NFE_API %enddef, otherwise it just generaates code tags instead of the parse tree.
* If a library contains a main, it seems that the target_include_directories() for the intended target that has that same main source will be ignored.
  * This leads to includes not being found by the compiler, because they are not passed to the compile line by cmake.
