#include <algorithm>
#include <fstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#ifdef _MSC_VER
#include <opencv2/ximgproc.hpp>
#endif

#include <functions.h>
#include <input.h>
#include <terminal.h>

std::vector<std::pair<std::string, cv::Rect>> text_getter(cv::Mat image)
{
    using namespace cv;
    using namespace cv::dnn;
    int width = 320;
    int height = 320;

#ifdef _MSC_VER
    TextDetectionModel_EAST detector("../../resources/frozen_east_text_detection.pb");
#else
    TextDetectionModel_EAST detector("../resources/frozen_east_text_detection.pb");
#endif
    detector.setConfidenceThreshold(0.5).setNMSThreshold(0.4);

    // Parameters for Detection
    double detScale = 1.0;
    Size detInputSize = Size(width, height);
    Scalar detMean = Scalar(123.68, 116.78, 103.94);
    bool swapRB = true;
    detector.setInputParams(detScale, detInputSize, detMean, swapRB);

    // Detection
    std::vector<std::vector<Point>> detResults;
    detector.detect(image, detResults);

    std::vector<std::pair<std::string, cv::Rect>> res;
    res.reserve(detResults.size());
    std::cout << detResults.size() << '\n';
    for (auto &i : detResults)
    {
        auto r = boundingRect(i);
        res.push_back({text_recognitor(image(r)), r});
    }
    return res;
}

std::string text_recognitor(const cv::Mat &text_image)
{
    static bool first_time = true;
    cv::imshow("2", text_image);
#ifdef _MSC_VER
    std::string rec_model_path = "../../resources/crnn_cs.onnx";
#else
    std::string rec_model_path = "../resources/crnn_cs.onnx";
#endif

    static cv::dnn::TextRecognitionModel recognizer(rec_model_path);
    if (first_time)
    {
        // Load vocabulary
        std::ifstream voc_file;
#ifdef _MSC_VER
        voc_file.open("../../resources/alphabet_94.txt");
#else
        voc_file.open("../resources/alphabet_94.txt");
#endif
        CV_Assert(voc_file.is_open());
        std::string voc_line;
        std::vector<std::string> vocabulary;
        while (std::getline(voc_file, voc_line))
        {
            vocabulary.push_back(std::move(voc_line));
        }
        recognizer.setVocabulary(vocabulary);
        recognizer.setDecodeType("CTC-greedy");
        // Parameters for Recognition
        double recScale = 1.0 / 127.5;
        cv::Scalar rec_mean = cv::Scalar(127.5, 127.5, 127.5);
        cv::Size rec_input_size = cv::Size(100, 32);
        recognizer.setInputParams(recScale, rec_input_size, rec_mean);
    }
    std::string recognition_result = recognizer.recognize(text_image);
    return recognition_result;
}

// offset between points, to be equal
constexpr size_t threshold = 10;

bool is_equal(cv::Point lhs, cv::Point rhs)
{
    return std::abs(lhs.x - rhs.x) < threshold && std::abs(lhs.y - rhs.y) < threshold;
}

bool on_line(line l, cv::Point p)
{
    double a = std::hypot(l.first.x - l.second.x, l.first.y - l.second.y);
    double b = std::hypot(l.first.x - p.x, l.first.y - p.y);
    double c = std::hypot(l.second.x - p.x, l.second.y - p.y);
    double per = (a + b + c) / 2;
    double h;
    if (b * b > a * a + c * c || c * c > a * a + b * b)
        h = std::min(b, c);
    else
        h = 2 * sqrt(per * (per - a) * (per - b) * (per - c)) / a;

    return h < threshold;
}

std::vector<line> get_lines(cv::Mat src, std::string s)
{
#ifdef _MSC_VER
    using namespace cv;
    using namespace cv::ximgproc;

    int length_threshold = 10;
    float distance_threshold = 1.41421356f;
    double canny_th1 = 50.0;
    double canny_th2 = 50.0;
    int canny_aperture_size = 3;
    bool do_merge = true;
    Ptr<FastLineDetector> fld = createFastLineDetector(
        length_threshold, distance_threshold, canny_th1, canny_th2, canny_aperture_size, do_merge);
    std::vector<Vec4f> lines;

    for (int run_count = 0; run_count < 5; run_count++)
    {
        double freq = getTickFrequency();
        lines.clear();
        int64 start = getTickCount();
        // Detect the lines with FLD
        fld->detect(src, lines);
        double duration_ms = double(getTickCount() - start) * 1000 / freq;
        std::cout << "Elapsed time for FLD " << duration_ms << " ms." << std::endl;
    }
    // Show found lines with FLD
    Mat line_image_fld(src);
    fld->drawSegments(line_image_fld, lines);
    std::vector<::line> res;
    res.reserve(lines.size());
    for (auto &line : lines)
        res.push_back({cv::Point(line[0], line[1]), cv::Point(line[2], line[3])});
    // for (auto &line : lines)
    //     imshow("FLD result", line_image_fld);
    // waitKey();
    return res;
#else

    // system("..\\resources\\VS.exe .\\flowchart3.jpg");
    system(("..\\resources\\VS.exe " + std::move(s)).data());
    std::vector<::line> res;
    std::ifstream fin("lines.txt");
    double x0, y0, x1, y1;
    while (fin >> x0 >> y0 >> x1 >> y1)
        res.push_back({cv::Point(x0, y0), cv::Point(x1, y1)});

    // std::remove("")
    return res;
#endif
}
