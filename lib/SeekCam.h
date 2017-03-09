/*
 *  Abstract seek camera class
 *  Author: Maarten Vandersteegen
 */

#ifndef SEEK_CAM_H
#define SEEK_CAM_H

#include "SeekDevice.h"
#include <opencv2/opencv.hpp>

namespace LibSeek {

class SeekCam
{
public:
    /*
     *  Initialize the camera
     *  Returns true on success
     */
    bool open();

    /*
     *  Initialize the camera
     *  ffc_filename:
     *      Filename for additional flat field calibration and corner
     *      gradient elimination. If provided and found, the image will
     *      be subtracted from each retrieved frame. If not, no additional
     *      flat field calibration will be applied
     *  Returns true on success
     */
    bool open(std::string ffc_filename);

    /*
     *  Returns true when camera is initialized
     */
    bool isOpened();

    /*
     *  Close the camera
     */
    void close();

    /*
     *  Grab a frame
     *  Returns true on success
     */
    bool grab();

    /*
     *  Retrieve the last grabbed 14-bit frame
     *  Returns true on success
     */
    bool retrieve(cv::Mat& dst);

    /*
     *  Convert a 14-bit thermal measurement to an
     *  enhanced 8-bit greyscale image for visual inspection
     */
    void convertToGreyScale(cv::Mat& src, cv::Mat& dst);

    /*
     *  Read grabs and retrieves a frame
     *  Returns true on success
     */
    bool read(cv::Mat& dst);

    /*
     *  Get the frame counter value
     */
    virtual int frame_counter() = 0;

protected:

    /*
     *  Methods
     */
    SeekCam(int vendor_id, int product_id, uint16_t* buffer, size_t raw_height, size_t raw_width, cv::Rect roi);
    ~SeekCam();

    virtual bool init_cam() = 0;
    virtual int frame_id() = 0;
    bool get_frame();
    void print_usb_data(vector<uint8_t>& data);
    void create_dead_pixel_list();
    void apply_dead_pixel_filter();
    uint16_t calc_mean_value(cv::Point p, int right_border, int lower_border);

    /*
     *  Variables
     */
    const int m_offset;

    bool m_is_opened;
    SeekDevice m_dev;
    uint16_t* m_raw_data;
    size_t m_raw_data_size;
    cv::Mat m_raw_frame;
    cv::Mat m_frame;
    cv::Mat m_flat_field_calibration_frame;
    cv::Mat m_additional_ffc;
    cv::Mat m_dead_pixel_mask;
    std::vector<cv::Point> m_dead_pixel_list;
};

} /* LibSeek */

#endif /* SEEK_CAM_H */
