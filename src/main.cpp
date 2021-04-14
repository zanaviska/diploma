#include <algorithm>
#include <iostream>
#include <set>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <functions.h>
#include <terminal.h>

int main(int argc, char **argv)
{
    // Loads an image
    cv::Mat src = cv::imread("../flowchart2.jpg", cv::IMREAD_GRAYSCALE);
    // Check if image is loaded fine
    if (src.empty())
    {
        printf(" Error opening image\n");
        return -1;
    }

    cv::Mat clone;
    cv::cvtColor(src, clone, cv::COLOR_GRAY2BGR);

// Initialize the parameters
    float confThreshold = 0.5; // Confidence threshold
    float nmsThreshold = 0.4;  // Non-maximum suppression threshold
    int inpWidth = 416;        // Width of network's input image
    int inpHeight = 416;       // Height of network's input image

    // Load names of classes
    std::string classesFile = "coco.names";
//    std::ifstream ifs(classesFile.c_str());
//    std::string line;
//    while (getline(ifs, line)) classes.push_back(line);

// Give the configuration and weight files for the model
    std::string modelConfiguration = "../resources/blocks.cfg";
    std::string modelWeights = ".yolov3.weights";

// Load the network
    Net net = readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    outputFile = "yolo_out_cpp.avi";
    if (parser.has("image"))
    {
        // Open the image file
        str = parser.get<String>("image");
        ifstream ifile(str);
        if (!ifile) throw("error");
        cap.open(str);
        str.replace(str.end()-4, str.end(), "_yolo_out_cpp.jpg");
        outputFile = str;
    }
    else if (parser.has("video"))
    {
        // Open the video file
        str = parser.get<String>("video");
        ifstream ifile(str);
        if (!ifile) throw("error");
        cap.open(str);
        str.replace(str.end()-4, str.end(), "_yolo_out_cpp.avi");
        outputFile = str;
    }
        // Open the webcaom
    else cap.open(parser.get<int>("device"));

    // Get the video writer initialized to save the output video
    if (!parser.has("image")) {
        video.open(outputFile, VideoWriter::fourcc('M','J','P','G'), 28, Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
    }

    // Process frames.
    while (waitKey(1) < 0)
    {
        // get frame from the video
        cap >> frame;

        // Stop the program if reached end of video
        if (frame.empty()) {
            cout << "Done processing !!!" << endl;
            cout << "Output file is stored as " << outputFile << endl;
            waitKey(3000);
            break;
        }
        // Create a 4D blob from a frame.
        blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth, inpHeight), Scalar(0,0,0), true, false);

        //Sets the input to the network
        net.setInput(blob);

        // Runs the forward pass to get output of the output layers
        vector<Mat> outs;
        net.forward(outs, getOutputsNames(net));

        // Remove the bounding boxes with low confidence
        postprocess(frame, outs);

        // Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
        vector<double> layersTimes;
        double freq = getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        string label = format("Inference time for a frame : %.2f ms", t);
        putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

        // Write the frame with the detection boxes
        Mat detectedFrame;
        frame.convertTo(detectedFrame, CV_8U);
        if (parser.has("image")) imwrite(outputFile, detectedFrame);
        else video.write(detectedFrame);

        imshow(kWinName, frame);
    }

    // Get the names of the output layers
    vector<String> getOutputsNames(const Net& net)
    {
        static vector<String> names;
        if (names.empty())
        {
            //Get the indices of the output layers, i.e. the layers with unconnected outputs
            vector<int> outLayers = net.getUnconnectedOutLayers();

            //get the names of all the layers in the network
            vector<String> layersNames = net.getLayerNames();

            // Get the names of the output layers in names
            names.resize(outLayers.size());
            for (size_t i = 0; i < outLayers.size(); ++i)
                names[i] = layersNames[outLayers[i] - 1];
        }
        return names;
    }

    // Remove the bounding boxes with low confidence using non-maxima suppression
    void postprocess(Mat& frame, const vector<Mat>& outs)
    {
        vector<int> classIds;
        vector<float> confidences;
        vector<Rect> boxes;

        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Scan through all the bounding boxes output from the network and keep only the
            // ones with high confidence scores. Assign the box's class label as the class
            // with the highest score for the box.
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double confidence;
                // Get the value and location of the maximum score
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }

        // Perform non maximum suppression to eliminate redundant overlapping boxes with
        // lower confidences
        vector<int> indices;
        NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
        for (size_t i = 0; i < indices.size(); ++i)
        {
            int idx = indices[i];
            Rect box = boxes[idx];
            drawPred(classIds[idx], confidences[idx], box.x, box.y,
                     box.x + box.width, box.y + box.height, frame);
        }
    }
    return 0;
    auto words = text_getter(clone);
    for (auto word : words)
    {
        cv::rectangle(clone, word.second, cv::Scalar(0, 0, 255), cv::FILLED);
        cv::rectangle(src, word.second, cv::Scalar(255, 255, 255), cv::FILLED);
    }

    auto lines = get_lines(src);

    // clear duplicates
    for (size_t i = 0; i < lines.size(); i++)
        for (size_t j = 0; j < lines.size(); j++)
            if (i != j)
            {
                // if lines are equal
                if ((is_equal(lines[i].first, lines[j].first) &&
                     is_equal(lines[i].second, lines[j].second)) ||
                    (is_equal(lines[i].first, lines[j].second) &&
                     is_equal(lines[i].second, lines[j].first)))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if lines are the same but one is shorter
                else if ((is_equal(lines[i].first, lines[j].first) &&
                          on_line(lines[i], lines[j].second)) ||
                         (is_equal(lines[i].second, lines[j].first) &&
                          on_line(lines[i], lines[j].second)) ||
                         (is_equal(lines[i].first, lines[j].second) &&
                          on_line(lines[i], lines[j].first)) ||
                         (is_equal(lines[i].second, lines[j].second) &&
                          on_line(lines[i], lines[j].first)))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if one line in the middle of another
                else if (on_line(lines[i], lines[j].first) && on_line(lines[i], lines[j].second))
                {
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                // if lines should be concatinated
                else if (on_line({lines[i].first, lines[j].first}, lines[i].second) &&
                         on_line({lines[i].first, lines[j].first}, lines[j].second) &&
                         on_line(lines[i], lines[j].second))
                {
                    lines[i].second = lines[j].first;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].first, lines[j].second}, lines[i].second) &&
                         on_line({lines[i].first, lines[j].second}, lines[j].first) &&
                         on_line(lines[i], lines[j].first))
                {
                    std::cout << "merge is here\n"
                              << lines[i].first << ' ' << lines[i].second << '\n'
                              << lines[j].first << ' ' << lines[j].second << "\n\n";
                    lines[i].second = lines[j].second;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].second, lines[j].first}, lines[i].first) &&
                         on_line({lines[i].second, lines[j].first}, lines[j].second) &&
                         on_line(lines[i], lines[j].second))
                {
                    lines[i].first = lines[j].first;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
                else if (on_line({lines[i].second, lines[j].second}, lines[i].first) &&
                         on_line({lines[i].second, lines[j].second}, lines[j].first) &&
                         on_line(lines[i], lines[j].first))
                {
                    lines[i].first = lines[j].second;
                    swap(lines[j], lines.back());
                    lines.pop_back();
                    j--;
                }
            }

    // sort elements and remove duplicates
    std::sort(lines.begin(), lines.end(),
              [](auto lhs, auto rhs)
              {
                  if (std::min(lhs.first.y, lhs.second.y) < std::min(rhs.first.y, rhs.second.y))
                      return true;
                  if (std::min(lhs.first.y, lhs.second.y) > std::min(rhs.first.y, rhs.second.y))
                      return false;
                  if (std::max(lhs.first.y, lhs.second.y) < std::max(rhs.first.y, rhs.second.y))
                      return true;
                  if (std::max(lhs.first.y, lhs.second.y) > std::max(rhs.first.y, rhs.second.y))
                      return false;
                  return std::min(lhs.first.x, lhs.second.x) < std::min(lhs.first.x, lhs.second.x);
              });

    //    auto it = std::unique(lines.begin(), lines.end(), [](line lhs, line rhs) {
    //        return (is_equal(lhs.first, rhs.first) && is_equal(lhs.second, rhs.second)) ||
    //               (is_equal(lhs.first, rhs.second) && is_equal(lhs.second, rhs.first));
    //    });
    //    lines.erase(it, lines.end());
    //    [59, 49] [163, 49]
    //    [50, 89] [172, 89]
    //    [109, 209] [109, 89]
    //    [53, 92] [169, 92]
    //    [113, 209] [238, 209]
    //    [239, 299] [239, 210]
    //    [111, 212] [242, 212]
    //    [113, 402] [113, 299]
    //    [111, 300] [242, 300]
    //    [108, 402] [109, 338]
    //    [58, 399] [164, 399]
    //    [50, 439] [172, 439]

    std::cout << "lines\n";
    for (auto &i : lines)
        std::cout << i.first << ' ' << i.second << '\n';

    for (size_t i = 0; i < lines.size(); i++)
        switch (i)
        {
        case 0:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(0, 255, 0), 2);
            break;
        case 1:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(0, 0, 255), 2);
            break;
        case 2:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(255, 0, 255), 2);
            break;
        case 3:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(0, 255, 255), 2);
            break;
        case 4:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(255, 255, 0), 2);
            break;
        case 5:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(255, 0, 0), 2);
            break;
        case 6:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(255, 150, 150), 2);
            break;
        case 7:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(150, 255, 150), 2);
            break;
        default:
            cv::line(clone, lines[i].first, lines[i].second, cv::Scalar(100, 100, 100), 2);
        }

    if (std::abs(lines[1].first.y - lines[1].second.y) >
        std::abs(lines[2].first.y - lines[2].second.y))
        std::swap(lines[1], lines[2]);

    auto start = make_terminal(lines, 0, 1);

    cv::imshow("", clone);
    cv::imshow("1", src);
    cv::waitKey();
    return 0;
}