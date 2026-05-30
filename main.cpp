#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat sourceImg, grayFrame, blurFrame, edgeFrame;
Mat thresholdFrame, dilatedFrame, warpedDoc, croppedDoc;

vector<Point> detectedCorners, orderedCorners;

const float DOC_WIDTH = 420.0f;
const float DOC_HEIGHT = 596.0f;


Mat processImage(const Mat& frame)
{
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    GaussianBlur(
        grayFrame,
        blurFrame,
        Size(3, 3),
        3,
        0
    );

    Canny(blurFrame, edgeFrame, 30, 90);

    Mat morphKernel =
        getStructuringElement(MORPH_RECT, Size(3, 3));

    dilate(edgeFrame, dilatedFrame, morphKernel);

    return dilatedFrame;
}


vector<Point> findDocumentContour(const Mat& binaryImg)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(
        binaryImg,
        contours,
        hierarchy,
        RETR_EXTERNAL,
        CHAIN_APPROX_SIMPLE
    );

    vector<vector<Point>> approxContours(contours.size());

    vector<Point> largestQuad;
    double largestArea = 0;

    for (size_t idx = 0; idx < contours.size(); idx++)
    {
        double area = contourArea(contours[idx]);

        if (area > 1200)
        {
            double perimeter =
                arcLength(contours[idx], true);

            approxPolyDP(
                contours[idx],
                approxContours[idx],
                0.02 * perimeter,
                true
            );

            if (
                area > largestArea &&
                approxContours[idx].size() == 4
            )
            {
                largestArea = area;

                largestQuad = {
                    approxContours[idx][0],
                    approxContours[idx][1],
                    approxContours[idx][2],
                    approxContours[idx][3]
                };
            }
        }
    }

    return largestQuad;
}


void displayPoints(
    const vector<Point>& points,
    Scalar markerColor)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        circle(
            sourceImg,
            points[i],
            10,
            markerColor,
            FILLED
        );

        putText(
            sourceImg,
            to_string(i),
            points[i],
            FONT_HERSHEY_PLAIN,
            3,
            markerColor,
            3
        );
    }
}


vector<Point> sortCorners(
    const vector<Point>& corners)
{
    vector<Point> ordered;

    vector<int> sums;
    vector<int> differences;

    for (int i = 0; i < 4; i++)
    {
        sums.push_back(
            corners[i].x + corners[i].y
        );

        differences.push_back(
            corners[i].x - corners[i].y
        );
    }

    ordered.push_back(
        corners[min_element(
            sums.begin(),
            sums.end()) - sums.begin()]
    );

    ordered.push_back(
        corners[max_element(
            differences.begin(),
            differences.end()) - differences.begin()]
    );

    ordered.push_back(
        corners[min_element(
            differences.begin(),
            differences.end()) - differences.begin()]
    );

    ordered.push_back(
        corners[max_element(
            sums.begin(),
            sums.end()) - sums.begin()]
    );

    return ordered;
}

Mat warpDocument(
    const Mat& frame,
    const vector<Point>& corners,
    float width,
    float height)
{
    Point2f srcPts[4] = {
        corners[0],
        corners[1],
        corners[2],
        corners[3]
    };

    Point2f dstPts[4] = {
        {0.0f, 0.0f},
        {width, 0.0f},
        {0.0f, height},
        {width, height}
    };

    Mat transformMatrix =
        getPerspectiveTransform(srcPts, dstPts);

    warpPerspective(
        frame,
        warpedDoc,
        transformMatrix,
        Point(width, height)
    );

    return warpedDoc;
}

int main()
{
    string imagePath = "Resources/paper.jpg";

    sourceImg = imread(imagePath);

    if (sourceImg.empty())
    {
        cout << "Unable to load image." << endl;
        return -1;
    }


    thresholdFrame = processImage(sourceImg);


    detectedCorners =
        findDocumentContour(thresholdFrame);

    orderedCorners =
        sortCorners(detectedCorners);

    warpedDoc = warpDocument(
        sourceImg,
        orderedCorners,
        DOC_WIDTH,
        DOC_HEIGHT
    );

    int borderOffset = 8;

    Rect cropRegion(
        borderOffset,
        borderOffset,
        DOC_WIDTH - (borderOffset * 2),
        DOC_HEIGHT - (borderOffset * 2)
    );

    croppedDoc = warpedDoc(cropRegion);

    imshow("Original Image", sourceImg);
    imshow("Scanned Document", croppedDoc);

    waitKey(0);
    return 0;
}