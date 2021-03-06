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
    SCANNERLIB_API bool init(std::string filePath) override;
	SCANNERLIB_API void stop() override;
	SCANNERLIB_API cv::Mat GetColorFrame(int delayMS = 0) override;
	SCANNERLIB_API cv::Mat GetDepthFrame(int delayMS = 0) override;
	SCANNERLIB_API cv::Mat GetAveragedDepthFrame(int numFramesAveraged = -1, std::vector<ColorSpacePoint>* colorPoints = nullptr) override;
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
	SCANNERLIB_API bool PollFrames();
	SCANNERLIB_API RenderSet ProcessFrames();
	
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

	static cv::Mat ConvertFrameToMat(const rs2::frame& f);
	static cv::Mat ConvertDepthFrameToMetersMat(const rs2::depth_frame & f);

private:
	rs2::pointcloud pc_;
	//rs2::pipeline pipe;
    std::shared_ptr<rs2::pipeline> pipe_;
	rs2::frameset frames_;
	std::vector<Filter> filters;
	std::vector<Task> tasks;
	cv::Mat depth_mat_;
	cv::Mat color_mat_;
};

#endif // REALSENSE_CAMERA_MANAGER_H
