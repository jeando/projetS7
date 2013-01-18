#include "MyAL.h"
#include<iostream>
#include<ios>
/*
*Classe qui gère la carte son: surcouche à la librairie OpenAL
*/


/**
* constructeur pour initialiser la librairie
*/
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
std::vector<double> MyAL::sample_to_double(const std::vector<ALshort>& samples)
{
	std::vector<double> output(samples.size());
	for(unsigned int i=0; i< samples.size(); i++)
	{
		output[i]=samples[i];
	}
	return output;
}

std::string MyAL::choisir_capture_device()
{
	std::cout << "capture devices :" << std::endl;
	return choisir_device(MyAL::get_capture_devices());	
}
std::string MyAL::choisir_device()
{
	std::cout << "devices :" << std::endl;
	return choisir_device(MyAL::get_devices());
}
std::string MyAL::choisir_device(std::vector<std::string> vs)
{
	int k(0);
	for(std::string s : vs)
		std::cout << "\t" << k++ << ") "<< s << std::endl;
	std::cout << "choix : ";
	if(vs.size()==0){
		std::cout << "aucun" << std::endl;
			throw std::ios_base::failure("enable to load drivers");
		return "";
	}
	if(vs.size()==1){
		std::cout << "0" << std::endl; 
		return vs[0];
	}
	int choix(-1);
	do{
		std::cin >> choix;
	}while(choix<0||choix>=vs.size());
	return vs[choix];
}
/**
* initialise la carte son (sortie)
*/
bool MyAL::ini_device(std::string _device)
{
    if(!(device = alcOpenDevice(_device.size()==0?nullptr:_device.c_str())))
        return false;
    if(!(context = alcCreateContext(device, nullptr)))
        return false;
    return alcMakeContextCurrent(context);
}

/**
* recupere la liste des peripheriques de sortie possibles
*/
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

/**
* recupere la liste des peripheriques d'entree possibles
*/
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

/**
* constructeur pour capturer un son
*/
AL_Capture::AL_Capture(std::string _device ,std::string _capture_device,
                   ALenum _format, ALsizei _sample_rate,
				   ALsizei _sample_size)
:MyAL(_device), format(_format), sample_rate(_sample_rate), sample_size(_sample_size)
{
    if(!ini_capture_device(_capture_device))
        throw std::ios_base::failure("Can't load capture device");
}
AL_Capture::~AL_Capture()
{
    alcCaptureCloseDevice(capture_device);
}

/**
* initialise le micro
*/
bool AL_Capture::ini_capture_device(std::string _device)
{
    if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_FALSE)
        return false;
    capture_device = alcCaptureOpenDevice(_device.size()==0?NULL:_device.c_str(), sample_rate, format, sample_size);//NULL : periph par defaut
    return capture_device;
}

ALsizei AL_Capture::getSampleRate()
{
	return sample_rate;
}
/**
*lance l'enregistrement
*/
void AL_Capture::start(std::vector<ALshort>& _samples, std::mutex* _mutex)
{
	//vide buffer
    ALCint samples_available;
    alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, 1, &samples_available);
    if (samples_available > 0)
    {
		std::vector<ALshort> tmp(samples_available);
        alcCaptureSamples(capture_device, &tmp[0], samples_available);
    }

    continuer = true;
    capture_samples = &_samples;
    alcCaptureStart(capture_device);
    if(_mutex)
        captureThread = std::thread(&AL_Capture::record_with_mutex, this, _mutex);
    else
        captureThread = std::thread(&AL_Capture::record_without_mutex, this);
}

/**
* capture "vraiment" le son
*/
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

/**
* on specifie qu'on ecrit sur un "truc" et qu'on est le seul a pouvoir le faire
* (pas de double acces)
*/
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

/**
* sans la securite
*/
void AL_Capture::record_without_mutex()
{
    std::chrono::milliseconds dura(20);
    while(continuer){
        capture();
      //  std::this_thread::sleep_for(dura);
    }
}

/**
* arret de la capture
*/
void AL_Capture::stop()
{
    alcCaptureStop(capture_device);
    continuer = false;
    captureThread.join();
}

/**
*enregistrement du son
*/
void AL_Capture::save_sound(const std::string& file_name)
{
    MyAL::save_sound(file_name, *capture_samples, sample_rate, 1);
}

/**
*enregistrement du son
*/
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
        std::cerr << "open file " <<file_name.c_str() << "failure ! Exit saving !" << std::endl;
        return;
    }
    sf_writef_short(file, &samples[0], samples.size());
    sf_close(file);
}
std::pair<std::vector<ALshort>, SF_INFO> MyAL::load_sound(std::string file_name)
{
	SF_INFO file_info;
//	std::ostringstream oss("");
//	oss << get_stockage_path()<<file_name;
	SNDFILE* file = sf_open((get_stockage_path() + file_name).c_str(),
			SFM_READ, &file_info);
	if(!file)
        throw std::ios_base::failure("Can't open sound file");
	ALsizei nb_samples = static_cast<ALsizei>(file_info.channels
			* file_info.frames);
//	ALsizei sample_rate = static_cast<ALsizei>(file_info.samplerate);
	std::vector<ALshort> samples(nb_samples);
	if(sf_read_short(file, &samples[0], nb_samples) < nb_samples)
        throw std::ios_base::failure("Can't load sound");
	sf_close(file);
	return std::pair<std::vector<ALshort>, SF_INFO>(samples, file_info);
}
AL_Play::AL_Play(std::string _device)
:MyAL(_device),indice_buffer(0),buffers(1)
{
	alGenSources(1, &source);
	alGenBuffers(buffers.size(), &buffers[0]);
}
AL_Play::~AL_Play()
{
	alSourceStop(source);
	ALint nb_queued;
	ALuint buffer;
	alGetSourcei(source, AL_BUFFERS_QUEUED, &nb_queued);
 	for (ALint i = 0; i < nb_queued; ++i)
		alSourceUnqueueBuffers(source, AL_BUFFER, &buffer);
	alSourcei(source, AL_BUFFER, 0);
	alDeleteBuffers(buffers.size(), &buffers[0]);
	alDeleteSources(1, &source);
}
void AL_Play::play()
{
	alSourcePlay(source);
}
void AL_Play::stop()
{
	alSourceStop(source);
}
bool AL_Play::is_playing()
{
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);
	return status == AL_PLAYING;
}
ALsizei AL_Play::getSampleRate()
{
	return static_cast<ALsizei>(sound_info.samplerate);
}
void AL_Play::put_sound_in_buffer(std::string name)
{
	std::pair<std::vector<ALshort>, SF_INFO> sound = MyAL::load_sound(name);
	samples = sound.first;
	sound_info = sound.second;

	ALenum format;
	switch(sound_info.channels)
	{
		case 1 : format = AL_FORMAT_MONO16; break;
		case 2 : format = AL_FORMAT_STEREO16; break;
	}
	alBufferData(buffers[indice_buffer], format,
			&samples[0], samples.size()*sizeof(ALshort),
			static_cast<ALsizei>(sound_info.samplerate));
	int erreur;
	if((erreur=alGetError()) != AL_NO_ERROR){
		std::cout << erreur << " != " << AL_NO_ERROR<< std::endl;
		return;
	}
	alSourceQueueBuffers(source, 1, &buffers[indice_buffer]);

}
AL_Stream_Capture::AL_Stream_Capture(std::string _device,
		std::string _capture_device,
        ALenum _format, ALsizei _sample_rate, ALsizei _sample_size)
:AL_Capture(_device, _capture_device, _format, _sample_rate, _sample_size),
	running(false)
{

}
AL_Stream_Capture::~AL_Stream_Capture()
{
}
void AL_Stream_Capture::start_stream_capture()
{
	running = true;
	AL_Capture::start(samples, &mutex_sample);
}

events_audio AL_Stream_Capture::poll_event(Joueur& joueur)
{
	std::vector<double> temp;
	int indice;
label_debut_poll_event:
    mutex_sample.lock();
	if(sample_rate*sizeof(ALshort)<samples.size())
	{
		for(unsigned int i = 0; i<sample_rate*sizeof(ALshort); i++)
		{
			temp.push_back(samples[i]);
		}
	}
	else
	{
    	mutex_sample.unlock();
		return RIEN;
	}

    mutex_sample.unlock();
	indice = indice_debut(equalize_spectrogramme(spectrogramme(temp,
				sample_rate)));
	if(indice == -1)
	{
    	mutex_sample.lock();
		samples.erase(samples.begin(), samples.begin()+sample_rate*sizeof(ALshort)/2);
    	mutex_sample.unlock();
		goto label_debut_poll_event;
	}
	if(indice > sample_rate*sizeof(ALshort)*4.0/10)
	{
    	mutex_sample.lock();
		samples.erase(samples.begin(),
				samples.begin()+indice-sample_rate*sizeof(ALshort)*4.0/10);
    	mutex_sample.unlock();
		goto label_debut_poll_event;
	}
	events_audio event = analyse(equalize_spectrogramme(
				spectrogramme(samples,sample_rate), joueur);
	if(event != RIEN)
	{
    	mutex_sample.lock();
		samples.erase(samples.begin(),
				samples.begin()+indice-sample_rate*sizeof(ALshort));
    	mutex_sample.unlock();

		return event;
	}
	std::cout << "erreur inconnu in file : " << __FILE__ << " in line " << __LINE__ << std::endl;
	return RIEN;	
}
events_audio AL_Stream_Capture::wait_event(Joueur& joueur)
{
	events_audio event;
	if(!running)
		start_stream_capture();
	do
	{
		event = poll_event(joueur);
	}while(event==RIEN);
	return event;
}
