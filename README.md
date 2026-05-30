# 📄 OpenCV Document Scanner

A simple document scanner built with **C++** and **OpenCV** that automatically detects a document in an image, applies a perspective transformation, and produces a clean scanned output.

## 🚀 Features

* Convert image to grayscale
* Noise reduction using Gaussian Blur
* Edge detection with Canny
* Automatic document contour detection
* Perspective correction (bird's-eye view)
* Border cropping for cleaner results
* Real-time visualization of scanned documents

## 🛠️ Technologies Used

* C++
* OpenCV

## 📂 Project Structure

```text
DocumentScanner/
│
├── Resources/
│   └── paper.jpg
│
├── main.cpp
├── README.md
└── CMakeLists.txt (optional)
```

## ⚙️ How It Works

The scanner follows these steps:

1. **Preprocessing**

   * Convert image to grayscale
   * Apply Gaussian Blur
   * Detect edges using Canny
   * Dilate edges to strengthen contours

2. **Document Detection**

   * Find external contours
   * Approximate contour shapes
   * Select the largest quadrilateral contour

3. **Corner Reordering**

   * Identify top-left, top-right, bottom-left, and bottom-right points

4. **Perspective Transformation**

   * Warp the image into a rectangular document view

5. **Cropping**

   * Remove unwanted border pixels

## 📸 Input

Place your image inside the `Resources` folder.

Example:

```cpp
string imagePath = "Resources/paper.jpg";
```

## ▶️ Build and Run

### Using g++

```bash
g++ main.cpp -o scanner `pkg-config --cflags --libs opencv4`
./scanner
```

### Using Visual Studio

1. Create a C++ Console Project.
2. Install OpenCV.
3. Configure OpenCV include and library directories.
4. Add `main.cpp`.
5. Build and run.

## 🖼️ Output

The application displays:

* Original Image
* Scanned Document

The scanned document is perspective-corrected and cropped for a cleaner appearance.

## 📈 Future Improvements

* Real-time webcam document scanning
* Automatic brightness and contrast adjustment
* PDF export support
* Multiple document detection
* Adaptive thresholding for enhanced readability

## 🤝 Contributing

Contributions, suggestions, and improvements are welcome. Feel free to fork the repository and submit a pull request.

## 📜 License

This project is open-source and available under the MIT License.
