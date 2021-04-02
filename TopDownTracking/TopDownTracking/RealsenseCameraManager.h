#ifndef REALSENSE_CAMERA_MANAGER_H
#define REALSENSE_CAMERA_MANAGER_H

#include <librealsense2/rs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "IMixedCameraManager.h"
#include "CVTask.h"

class RealsenseCameraManager : public IMixedCameraManager
{
public:
	SCANNERLIB_API RealsenseCameraManager();
	SCANNERLIB_API ~RealsenseCameraManager() override;
    SCANNERLIB_API bool init() override;
    SCANNERLIB_API bool init(std::string filePath = "") override;
	SCANNERLIB_API void stop() override;
	SCANNERLIB_API cv::Mat getColorFrame(int delayMS = 0) override;
	SCANNERLIB_API cv::Mat getDepthFrame(int delayMS = 0) override;
	SCANNERLIB_API cv::Mat getAveragedDepthFrame(int numFramesAveraged = -1, std::vector<ColorSpacePoint>* colorPoints = nullptr) override;
	SCANNERLIB_API double getTableAt(double x, double y) override;
	SCANNERLIB_API cv::Point3d get3DFromDepthAt(double x, double y, double depth) override;

public:
	typedef struct RenderSet {
		rs2::points points;
		rs2::depth_frame depthFrame;
		rs2::video_frame colorFrame;
        cv::Mat cvDepthFrame;
        cv::Mat cvColorFrame;
	} RenderSet;
	SCANNERLIB_API bool pollFrames();
	SCANNERLIB_API RenderSet processFrames();
	
	typedef struct Filter {
		rs2::filter filter;
		rs2_stream type;
		bool enabled = true;
	} Filter;
	SCANNERLIB_API int addFilter(Filter filter);
	SCANNERLIB_API void enableFilter(int filterId, bool enabled);

    typedef struct Task {
        CVTask *task;
        bool enabled = true;
    } Task;
    SCANNERLIB_API int addTask(Task task);
    SCANNERLIB_API void enableTask(int taskId, bool enabled);

	static cv::Mat convertFrameToMat(const rs2::frame& f);
	static cv::Mat convertDepthFrameToMetersMat(const rs2::depth_frame & f);

private:
	rs2::pointcloud pc;
	//rs2::pipeline pipe;
    std::shared_ptr<rs2::pipeline> pipe;
	rs2::frameset frames;
	std::vector<Filter> filters;
	std::vector<Task> tasks;
	cv::Mat depthMat;
	cv::Mat colorMat;
};

#endif // REALSENSE_CAMERA_MANAGER_H