# ClipDrop

<p align="center">
    <img src="https://img.shields.io/badge/QT-ClipDrop-41CD52?style=for-the-badge&logo=qt" alt="ClipDrop" width="300">
</p>

<p align="center">
    <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
    <img src="https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt">
    <img src="https://img.shields.io/badge/SQLite-003B57?style=for-the-badge&logo=sqlite&logoColor=white" alt="SQLite">
    <img src="https://img.shields.io/badge/QHotkey-FF6B35?style=for-the-badge" alt="QHotkey">
    <img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake">
</p>

## 📝 About

ClipDrop is a clipboard manager implemented in C++ using the Qt framework. In contrast to standard clipboard managers, ClipDrop automatically categorizes and saves your clipboard items, making it simple to find and reuse previously copied items. This is my first project using C++ and the Qt framework.

Text, code snippets, URLs, colors, photos and files are all automatically detected and categorized by the application's content analysis algorithm. In addition to a search bar and filtering options, it includes a global hotkey for instant access to your clipboard history.

This project was developed as the final project for CS50, a computer science course at Harvard University.

## 🛠️ Features

- **Smart Content Detection**: automatically categorizes clipboard content into text, code, links, colors, images, and files
- **Global Hotkey**: hit `Ctrl+Space` to bring up your clipboard history
- **Search**: search through your clipboard history
- **Content Type Filtering**: filter items by specific content types (e.g. only images, only links)
- **Persistent Storage**: your history is stored in SQLite, so it’s safe even after restarting the app
- **Visual Previews**: color samples and image thumbnails for visual content
- **One-Click Restore**: easily restore any item back to your clipboard
- **Clean Interface**: a modern, semi-transparent user interface that shows where your cursor is

## 🎥 See ClipDrop in Action

<p align="center">
    <img src=".github/images/clipdrop_clipboard.jpeg" alt="ClipDrop Clipboard" width="1000">
    <br>
    <em>Manage your clipboard history effortlessly</em>
</p>
<p align="center">
    <img src=".github/images/clipdrop_colors.jpeg" alt="Colors" width="1000">
    <br>
    <em>Color Detection - ClipDrop detects copied colors and shows them as previews</em>
</p>
<p align="center">
    <img src=".github/images/clipdrop_files.jpeg" alt="Files" width="1000">
    <br>
    <em>File Detection - ClipDrop automatically detects and categorizes copied files</em>
</p>
<p align="center">
    <img src=".github/images/clipdrop_search.jpeg" alt="Search" width="1000">
    <br>
    <em>Search - Quickly find any item in your clipboard history</em>
</p>
<p align="center">
    <img src=".github/images/clipdrop_filtering.jpeg" alt="Filtering" width="1000">
    <br>
    <em>Content Type Filtering - View only specific content types in your clipboard history</em>
</p>

<p align="center">

  https://github.com/user-attachments/assets/25a21752-329c-4af7-ada5-0624318920b4

</p>
<p align="center">
  <em>Video demonstration - showing ClipDrop in action</em>
</p>

## 🏗️ Project Architecture

### Core Components

**MainWindow (`mainwindow.h/cpp`)**: This is the main part of the app, it manages the user interface and interactions between different components. It handles the global hotkey, clipboard monitoring, and UI updates. The window is frameless with custom mouse handling for dragging and automatic positioning at the cursor location.

**ClipboardItem (`src/widgets/clipboarditem.h/cpp`)**: A custom widget that represents each clipboard entry in the interface. Each item displays content previews, allows you to restore or delete items and adjusts its appearance based on content type. The widget handles visual representation and user interactions for clipboard entries.

### Data Models

**ClipboardData (`src/models/clipboarddata.h/cpp`)**: The data model represents a clipboard entry with properties such as content type, plaintext, content, images, and creation timestamps. This class is used for all clipboard operations and database interactions.

**ContentType (`src/models/contenttype.h`)**: An enumeration that defines the various content types the application can recognize and handle.

### Services Layer

**ContentAnalyzer (`src/services/contentanalyzer.h/cpp`)**: The content analysis algorithm is used to determine the type of clipboard text. It uses algorithms to detect code snippets by analyzing symbol density and keyword frequency, identifies URLs through pattern matching, and validates color values in various formats.

**ContentConverter (`src/services/contentconverter.h/cpp`)**: Handles conversion between Qt's QImage format and binary data for database storage.

### Database Layer

**ClipboardDatabase (`src/db/clipboarddatabase.h/cpp`)**: Manages all database operations using SQLite for persistent storage. It handles CRUD operations for clipboard entries, supports querying with search and filtering capabilities, and manages image data as BLOBs.

### External Dependencies

**QHotkey Library (`libs/qhotkey/`)**: A third-party library that extends Qt's hotkey capabilities to work outside of the application window. Since Qt's built-in QShortcut only works when the application has focus, QHotkey enables system-wide global shortcuts that are important for clipboard manager functionality.

## 🧩 Design Decisions and Challenges

### Global Hotkey Implementation
One of the primary challenges was implementing global hotkeys since Qt's native QShortcut class only functions when the application window has focus. This limitation would significantly restrict the usability of a clipboard manager. After researching available solutions, I chose the QHotkey library for its cross-platform compatibility.

### Content Type Detection
The content analysis system required careful consideration of detection algorithms. For code detection, I implemented a hybrid approach analyzing both symbol density (special characters like braces, semicolons) and keyword frequency (programming language keywords). This two-criteria system reduces false positives while still achieving good detection accuracy for different programming languages and coding styles.

### Database Schema Design
I chose SQLite for data storage because it is lightweight and requires no configuration. The schema stores content (plaintext, content, images as BLOBs) with type indicators. Timestamps help organize content chronologically in the user interface.

### User Interface
The application appears as a translucent overlay at the cursor position, disappears when focus is lost, and uses visual cues like icons and previews to quickly identify content types. The frameless window with custom dragging gives the application a modern appearance.

### Memory Management
Special attention was paid to proper memory management. The application implements proper cleanup in destructors to prevent memory leaks.

## 🛠️ Technical Implementation

The application initialization process has several steps: database connection, global hotkey setup, clipboard monitoring setup, and UI styling. The clipboard monitoring system uses Qt's QClipboard signals to detect changes, analyzes new content through the ContentAnalyzer, creates appropriate ClipboardData objects, stores them in the database, and updates the UI.

Content detection algorithms are particularly sophisticated for code recognition. They examine character patterns and keyword density to differentiate between regular text and programming code. The color detection system supports multiple formats including hex codes (with or without #), and named colors.

The search and filtering system provides real-time results by combining SQL LIKE queries for text search with type-based filtering.

## 🏁 Getting Started

### Prerequisites

- Qt 6.x framework
- C++17 compatible compiler
- CMake 3.16 or higher
- SQLite (included with Qt)

### Building the Project

1. Clone the repository
   ```bash
   git clone https://github.com/The5TiM/clipdrop.git
   cd clipdrop
   ```

2. Create build directory
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake
   ```bash
   cmake -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/<Version>/mingw_64" ..
   ```

4. Build the application
   ```bash
   cmake --build .
   ```

### Usage

1. Launch ClipDrop
2. Copy any content to your clipboard (text, images, files)
3. Press `Ctrl+Space` to open the clipboard history
4. Use the search bar to find specific items
5. Filter by content type using the dropdown
6. Click any item to restore it to your clipboard
7. Press `Ctrl+Space` again or click elsewhere to close

## 🧠 Project Learning Outcomes

This project helped me gain experience in the following areas:

- **Qt Framework**: I learned how to work with signals and slots, build custom widgets, and layout management.
- **C++ Memory Management**: I gained a better understanding of using pointers and preventing memory leaks.
- **Database Integration**: I learned about SQLite usage, BLOB handling for binary data, and query optimization.
- **Third-Party Library Integration**: I gained experience with CMake configuration and library linking.
- **User Experience Design**: I focused on creating a clean and intuitive interface that is easy to use.

## 🧾 License and Acknowledgments

**ClipDrop** © 2025 Juwan Jouma  
This project is licensed under the GNU General Public License (GPL).

This project uses third-party open-source components:

- **QHotkey** © 2016 Felix Barz  
Licensed under the BSD 3-Clause License.  

## 🧑‍💻 Author

<p align="center">
    <a href="https://github.com/The5TiM"><img src="https://img.shields.io/badge/The5TiM%20|%20Juwan%20Jouma-181717?style=for-the-badge&logo=github" alt="The5TiM | Juwan Jouma" /></a>
</p>
