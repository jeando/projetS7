#include "MyAL.h"
#include<iostream>
MyAL::MyAL(std::string _device)
:device(nullptr),context(nullptr)
{
    if(!ini_device(_device))
        throw std::ios_base::failure("Can't load device");
}

MyAL::~MyAL()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool MyAL::ini_device(std::string _device)
{
    if(!(device = alcOpenDevice(_device.size()==0?nullptr:_device.c_str())))
        return false;
    if(!(context = alcCreateContext(device, nullptr)))
        return false;
    return alcMakeContextCurrent(context);
}
std::vector<std::string> MyAL::get_devices()
{
    std::vector<std::string> devices;
    const ALCchar* device_list = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if (device_list)
    {
        while (strlen(device_list) > 0)
        {
            devices.push_back(device_list);
            device_list += strlen(device_list) + 1;
        }
    }
    return devices;
}
std::vector<std::string> MyAL::get_capture_devices()
{
    std::vector<std::string> devices;
    const ALCchar* device_list = alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER);
    if (device_list)
    {
        while (strlen(device_list) > 0)
        {
            devices.push_back(device_list);
            device_list += strlen(device_list) + 1;
        }
    }
    return devices;
}
AL_Capture::AL_Capture(std::string _device ,std::string _capture_device,
                   ALenum _format, ALsizei _sample_rate, ALsizei _sample_size)
:MyAL(_device), format(_format), sample_rate(_sample_rate), sample_size(_sample_size)
{
    if(!ini_capture_device(_capture_device))
        throw std::ios_base::failure("Can't load capture device");
}
AL_Capture::~AL_Capture()
{
    alcCaptureCloseDevice(capture_device);
}
bool AL_Capture::ini_capture_device(std::string _device)
{
    if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_FALSE)
        return false;
    capture_device = alcCaptureOpenDevice(_device.size()==0?NULL:_device.c_str(), sample_rate, format, sample_size);//NULL : periph par defaut
    return capture_device;
}
void AL_Capture::start(std::vector<ALshort>& _samples, std::mutex* _mutex)
{
    continuer = true;
    capture_samples = &_samples;
    alcCaptureStart(capture_device);
    if(_mutex)
        captureThread = std::thread(&AL_Capture::record_with_mutex, this, _mutex);
    else
        captureThread = std::thread(&AL_Capture::record_without_mutex, this);
}
inline void AL_Capture::capture()
{
    //*
    ALCint samples_available;
    alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, 1, &samples_available);
    if (samples_available > 0)
    {
        std::size_t start = capture_samples->size();
        capture_samples->resize(start + samples_available);
        alcCaptureSamples(capture_device, &((*capture_samples)[start]), samples_available);
    }//*/
}
void AL_Capture::record_with_mutex(std::mutex* _mutex)
{
    std::chrono::milliseconds dura(20);
    while(continuer){
        _mutex->lock();
        capture();
        _mutex->unlock();
        std::this_thread::sleep_for(dura);
    }
}
void AL_Capture::record_without_mutex()
{
    std::chrono::milliseconds dura(20);
    while(continuer){
        capture();
      //  std::this_thread::sleep_for(dura);
    }
}
void AL_Capture::stop()
{
    alcCaptureStop(capture_device);
    continuer = false;
    captureThread.join();
}
void AL_Capture::save_sound(const std::string& file_name)
{
    MyAL::save_sound(file_name, *capture_samples, sample_rate, 1);
}
void MyAL::save_sound(const std::string& file_name, const std::vector<ALshort>& samples,
                   ALsizei _sample_rate, ALsizei _nb_channel)
{
    SF_INFO file_info;
    file_info.frames   = 0;
    file_info.channels   = _nb_channel;
    file_info.samplerate = _sample_rate;
    file_info.format     = SF_FORMAT_PCM_16 | SF_FORMAT_WAV;
    file_info.sections   = 0;
    file_info.seekable   = 0;
    if (!sf_format_check(&file_info)){
        std::cerr << "Format parameter are uncorrect ! Exit saving !" << std::endl;
        return;
    }
    SNDFILE* file = sf_open(file_name.c_str(), SFM_WRITE, &file_info);
    if (!file){
        std::cerr << "open file failure ! Exit saving !" << std::endl;
        return;
    }
    sf_writef_short(file, &samples[0], samples.size());
    sf_close(file);
}
AL_Play::AL_Play(std::string _device)
:MyAL(_device)
{

}
