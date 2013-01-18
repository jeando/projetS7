#ifndef MYAL_H
#define MYAL_H

#include <AL/al.h>
#include <AL/alc.h>

#include<sndfile.h>

#include<string>
#include<vector>

#include<thread>
#include<mutex>

#include<cstring>

#include<ios>
#define get_stockage_path() ""
//"/home/favreau/projetS7/src/"
#define MAX_ALSHORT_VAL  ((1<<((sizeof(ALshort)<<3)-1))-1)
#define MIN_ALSHORT_VAL  (-(1<<((sizeof(ALshort)<<3)-1)))
enum events_audio {SON_HAUT, SON_BAS, SON_GAUCHE, SON_DROITE, RIEN};


class MyAL
{
	private:
		static std::string choisir_device(std::vector<std::string> lst_device);
    public:
		static std::vector<double> sample_to_double(const std::vector<ALshort>& samples);
		static std::string choisir_device();
		static std::string choisir_capture_device();
        static std::vector<std::string> get_devices();
        static std::vector<std::string> get_capture_devices();
        static std::pair<std::vector<ALshort>, SF_INFO> load_sound(std::string file_name);
        static void save_sound(const std::string& file_name, const std::vector<ALshort>& samples,ALsizei _sample_rate = 8000, ALsizei _nb_channel = 1);
    public:
        MyAL(std::string _device="");
        virtual ~MyAL();
		virtual ALsizei getSampleRate() = 0;
    protected:
        ALCdevice* device;
        ALCcontext* context;
    private:
        bool ini_device(std::string _device = "");
};
class AL_Capture : public MyAL
{
    public:
        AL_Capture(std::string _device = "",std::string _capture_device = "",
                   ALenum _format = AL_FORMAT_MONO16, ALsizei _sample_rate = 8000, ALsizei _sample_size = 1024/*8000*2*1*/);
        virtual ~AL_Capture();
        void start(std::vector<ALshort>& samples, std::mutex* mutex_of_sample = nullptr);
        void stop();
        void save_sound(const std::string& file_name);
		ALsizei getSampleRate();
    protected:
        ALCdevice* capture_device;
        ALenum format;//     = AL_FORMAT_MONO16;
        ALsizei sample_rate;// = 8000;//44100;//frqPerSecond
        ALsizei sample_size;// = SampleRate*2*1*5;//2 => short 2byte 16bit; 1 => 1channel => mono
    private:
        bool ini_capture_device(std::string _capture_device = "");
        void record_without_mutex(void);
        void record_with_mutex(std::mutex* mutex);
        inline void capture();
        std::thread captureThread;
        std::vector<ALshort>* capture_samples;
        bool continuer;
    //captureThread.join();
};
class AL_Play : public MyAL
{
    public:
        AL_Play(std::string _device="");
		virtual ~AL_Play();
		void play();
        void stop();
		void put_sound_in_buffer(std::string name);
		bool is_playing();
		ALsizei getSampleRate();
		
    protected:
    private:
		unsigned int indice_buffer;
		std::vector<ALshort> samples;
		SF_INFO sound_info;
		ALuint source;
		std::vector<ALuint> buffers;
};
class AL_Stream_Capture : AL_Capture
{
	public:
        AL_Capture(std::string _device = "",
				std::string _capture_device = "",
				ALenum _format = AL_FORMAT_MONO16,
				ALsizei _sample_rate = 8000,
				ALsizei _sample_size = 1024/*8000*2*1*/);
		events_audio poll_event();
		events_audio wait_event();
		void start_stream_capture();
	private:
		bool running;
		std::vector<ALshort> samples;

};
class AL_Stream_Play : public AL_Play
{

};
class AL_Stream_Capture_and_Play : public AL_Stream_Play,virtual public AL_Capture
{

};
#endif // MYAL_H
