
#include <AL/al.h>
#include <AL/alc.h>
#include<vector>
#include<complex>
#include<string>
#include<sstream>
#include<cstring>
#include<iostream>
#include<iomanip>
#include<thread>
#include <mutex>
#include<chrono>
#include<cmath>
#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"ft.h"
//#include<cstdio>
#include <stdexcept>
#include<fstream>
#include<cstdlib>

// "<<3" = "*8"
#define MAX_ALSHORT_VAL  ((1<<((sizeof(ALshort)<<3)-1))-1)

#define MIN_ALSHORT_VAL  (-(1<<((sizeof(ALshort)<<3)-1)))

ALCdevice* CaptureDevice;
void getDevices(std::vector<std::string>& devices)
{
    devices.clear();
    const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

    if (deviceList)
    {
        while (strlen(deviceList) > 0)
        {
            devices.push_back(deviceList);
            deviceList += strlen(deviceList) + 1;
        }
    }
}

void GetCaptureDevices(std::vector<std::string>& Devices)
{
    Devices.clear();
    const ALCchar* DeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    if (DeviceList)
    {
        while (strlen(DeviceList) > 0)
        {
            Devices.push_back(DeviceList);
            DeviceList += strlen(DeviceList) + 1;
        }
    }
}
bool initOpenAL()
{
    std::vector<std::string> lstDevices;
    getDevices(lstDevices);
    unsigned int i=0;
    if(lstDevices.size()>1){
        std::cout << "Liste des devices :" << std::endl;
        for(;i!=lstDevices.size(); i++){
            std::cout << "(" << i << ")" << ": " << lstDevices[i] << std::endl;
        }
        std::cout << "Choisissez votre sortie audio"<< std::endl;
        std::cin >> i;
    }
    // Ouverture du device

    ALCdevice* device = alcOpenDevice(lstDevices[i].c_str());//ou NULL pour en avoir un par default
    if (!device)
        return false;

    // Création du contexte
    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context)
        return false;

    // Activation du contexte
    if (!alcMakeContextCurrent(context))
        return false;

    return true;
}
ALenum         Format     = AL_FORMAT_MONO16;
ALsizei        SampleRate = 8000;//44100;//frqPerSecond
ALsizei        SampleSize = SampleRate*2*1*5;//2 => short 2byte 16bit; 1 => 1channel => mono
bool InitCapture()
{
    ALCcontext* context = alcGetCurrentContext();
    ALCdevice*  device  = alcGetContextsDevice(context);
    if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_FALSE)
        return false;
    CaptureDevice = alcCaptureOpenDevice(NULL, SampleRate, Format, 1024);//NULL : periph par defaut
    if (!CaptureDevice)
        return false;

    return true;
}
void ShutdownCapture()
{
    alcCaptureCloseDevice(CaptureDevice);
}

void shutdownOpenAL()
{
    ALCcontext* context = alcGetCurrentContext();
    ALCdevice*  device  = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
#include<sndfile.h>
ALuint loadSound(const std::string& Filename,ALuint Buffer)
{
    SF_INFO FileInfos;
    SNDFILE* File = sf_open(("/home/favreau/voix/"+Filename).c_str(), SFM_READ, &FileInfos);
    if (!File)
        return 0;
    ALsizei NbSamples  = static_cast<ALsizei>(FileInfos.channels * FileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(FileInfos.samplerate);
    // Lecture des échantillons audio au format entier 16 bits signé (le plus commun)
    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(File, &Samples[0], NbSamples) < NbSamples)
        return 0;
    sf_close(File);
    ALenum Format;
    switch (FileInfos.channels)
    {
        case 1 :  Format = AL_FORMAT_MONO16;   break;
        case 2 :  Format = AL_FORMAT_STEREO16; break;
        default : return 0;
    }
    alBufferData(Buffer, Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);
    int erreur;
    if ((erreur=alGetError()) != AL_NO_ERROR){
            std::cout << erreur << " != " << AL_NO_ERROR<< std::endl;
        return 0;
    }
    return Buffer;
}

void SaveSound(const std::string& Filename, const std::vector<ALshort>& Samples)
{
    SF_INFO FileInfos;
    FileInfos.frames   = 0;
    FileInfos.channels   = 1;
    FileInfos.samplerate = SampleRate;
    FileInfos.format     = SF_FORMAT_PCM_16 | SF_FORMAT_WAV;
    FileInfos.sections   = 0;
    FileInfos.seekable   = 0;
    if (!sf_format_check(&FileInfos)){
        std::cout << "Format parameter are uncorrect ! Exit saving !" << std::endl;
        return;
    }
    SNDFILE* File = sf_open(Filename.c_str(), SFM_WRITE, &FileInfos);
    if (!File)
        return;
    sf_writef_short(File, &Samples[0], Samples.size());
    sf_close(File);
}
void ReadMicroCapture(ALuint Buffer, ALsizei NbSamples)
{
    std::cout << "zerty" << std::endl;
    std::vector<ALshort> Samples(NbSamples);
    ALsizei TotalRead  = 0;
//    ALsizei TotalSize  = NbSamples * sizeof(ALshort);
    ALCint SamplesAvailable;
    while(TotalRead<NbSamples){
        alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
        if (SamplesAvailable > 0)
        {
            std::size_t Start = Samples.size();
            Samples.resize(Start + SamplesAvailable);
            alcCaptureSamples(CaptureDevice, &Samples[Start], ((ALsizei)Start+SamplesAvailable>NbSamples)?NbSamples-Start:SamplesAvailable);
            TotalRead+=SamplesAvailable*sizeof(ALshort);
        }
    }
    std::cout << TotalRead << std::endl;
    alBufferData(Buffer, Format, &Samples[0], TotalRead, SampleRate);
}
/*
void tampon()
{
    ALint Status;
    do
    {
        // On récupère le nombre de tampons qui ont été traités (ie. qui sont prêts à être réutilisés)
        ALint NbProcessed;
        alGetSourcei(Source, AL_BUFFERS_PROCESSED, &NbProcessed);

        // On les re-remplit et on les réinjecte dans la file
        for (ALint i = 0; i < NbProcessed; ++i)
        {
            // On sort le tampon de la file
            ALuint Buffer;
            alSourceUnqueueBuffers(Source, 1, &Buffer);

            // On le remplit avec les données du fichier
            ReadOgg(Buffer, 44100);

            // On le réinjecte dans la file
            alSourceQueueBuffers(Source, 1, &Buffer);
        }

        // On récupère l'état de la source
        alGetSourcei(Source, AL_SOURCE_STATE, &Status);
    }
    while (Status == AL_PLAYING);
}//*/
void capture2(std::vector<ALshort>& Samples)
{
        ALCint SamplesAvailable;
        alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
        if (SamplesAvailable > 0)
        {
            std::size_t Start = Samples.size();
            Samples.resize(Start + SamplesAvailable);
            alcCaptureSamples(CaptureDevice, &Samples[Start], SamplesAvailable);
        }
}
std::vector<ALshort> Samples;
std::mutex mutex_Samples;
bool continuer = true;
void capture()
{
    while(continuer){
        mutex_Samples.lock();
        capture2(Samples);
        mutex_Samples.unlock();
        std::chrono::milliseconds dura(20);
        std::this_thread::sleep_for(dura);
    }
}void decaler(std::vector<double>& v)
{
    double temp;
    for(unsigned int i=0;i<v.size()/2;i++){
        //std::cout << i<< std::endl;
        temp=v[i+v.size()/2];
        v[i+v.size()/2]=v[i];
        v[i]=temp;
    }
}
SDL_Surface* screen(NULL);

void traitement(std::vector<double> y)
{
//    static int zzz = 0;
            /*for(ALshort& sample : Samples){
                y.push_back(sample);
            }//*/
//autocorrelation
           /* std::vector<double> autocorrelation;
            for(unsigned int tau = 0; tau<y.size(); tau++)
            {
                double temp(0);
                for(unsigned t = tau; t<y.size(); t++)
                {
                    temp+=y[t]*y[t-tau];
                }
                autocorrelation.push_back(temp);
            }

            std::vector<std::complex<double>> aaa = fft(autocorrelation);
            //*/
			std::vector<std::complex<double>> aaa = fft(y);
            double ymax=0;
            for(unsigned int k=0;k<y.size();k++){
                y[k]=log(sqrt(std::norm(aaa[k])));
                y[k]=sqrt(std::norm(aaa[k]));
                if(y[k]>ymax)ymax=y[k];
            }
            decaler(y);
            std::vector<double> frq;
            for(unsigned int k=0; k<y.size(); k++){
                frq.push_back(SampleRate*(k*1./y.size()-1./2));
            }
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
            for(int i=0; i<200; i++)
            {
                vlineRGBA(screen, i, 400, 400-y[i]/ymax*400.0,y[i]/ymax*255.0, y[i]/ymax*255.0-255, 0, 255);
            }
        //    std::cout << ymax << std::endl;
            SDL_Flip(screen);
}
int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(200,400,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    initOpenAL();
    InitCapture();
    ALuint Source;
    alGenSources(1, &Source);
    ALuint Buffers[2];
    alGenBuffers(2, Buffers);
    //alBufferData(Buffer, Format, &Samples[0], Samples.size() * sizeof(ALushort), SampleRate);
    alcCaptureStart(CaptureDevice);

    std::thread captureThread(capture);
    alBufferData(Buffers[0], Format, &Samples[0], Samples.size() * sizeof(ALushort), SampleRate);
   // std::cout << Samples.size() << std::endl;
    //Samples.clear();
    //capture();
    /*alBufferData(Buffers[1], Format, &Samples[0], Samples.size() * sizeof(ALushort), SampleRate);
    std::cout << Samples.size() << std::endl;
    Samples.clear();//*/
    alSourceQueueBuffers(Source, 1, Buffers);
    alSourcePlay(Source);
//    ALint NbProcessed;
    ALint Status;
//    int time_start = time(NULL);
    //int i=1;
    //ALint start = 0;
    //ALint position = 0;
    std::chrono::milliseconds dura(20);
    std::vector<double> y(200);
    bool continuer2 = true;
    SDL_Event event;
    int key;
    int lataille;
    while(continuer2){//time(NULL) < time_start+15){
            //std::this_thread::sleep_for(dura);
            mutex_Samples.lock();//lock to check size
            lataille = Samples.size();
            mutex_Samples.unlock();
            if(lataille>200){
                mutex_Samples.lock();
                for(unsigned int i=0;i<200;i++){
                    y[i]=Samples[i]*1.0/MAX_ALSHORT_VAL;
                }
                Samples.erase(Samples.begin(), Samples.begin()+200);
                mutex_Samples.unlock();
                    traitement(y);
            }else{
                std::this_thread::sleep_for(dura);

            }

            SDL_PollEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:continuer2 = false;break;
                case SDL_KEYDOWN:
                    key=event.key.keysym.sym;
                    switch(key)
                    {
                        case SDLK_c:
                            mutex_Samples.lock();
                            Samples.clear();
                            mutex_Samples.unlock();
                        break;
                    }
            }
        /*alGetSourcei(Source, AL_SOURCE_STATE, &Status);
        if((Status != AL_PLAYING)){
            alGetSourcei(Source, AL_SAMPLE_OFFSET, &start);
            position+=start;
            std::cout << start<< " " << position << " " << Samples.size() << std::endl;
            mutex_Samples.lock();
            if(Samples.size()==0){
                 mutex_Samples.unlock();
                continue;
            }
            alBufferData(Buffers[i], Format, &Samples[position], (Samples.size()-position)* sizeof(ALushort), SampleRate);
            //copie, enlever bruit, fft, traitement ...
            int n = log2(Samples.size());
            n=pow(2,n);
            std::vector<double> y;
            std::cout << n << " " <<Samples.size()-n <<std::endl;
            for(int k = 0; k<n; k++)
                y.push_back(Samples[k]);
        traitement(y);


            //Samples.clear();
            Samples.erase(Samples.begin(), Samples.end()-(Samples.size()-n));
            position = Samples.size();
            //alBufferData(Buffers[i], Format, &Samples[start], (Samples.size()-start)* sizeof(ALushort), SampleRate);
           // alBufferData(Buffers[i], Format, &Samples[0], (Samples.size())* sizeof(ALushort), SampleRate);
            mutex_Samples.unlock();
            alSourceQueueBuffers(Source, 1, &Buffers[i]);
            i^=1;
            alSourceUnqueueBuffers(Source, 1, &Buffers[i]);
            alGetSourcei(Source, AL_SOURCE_STATE, &Status);
            if(Status != AL_PLAYING)
                alSourcePlay(Source);
        }//*/
    }
    alcCaptureStop(CaptureDevice);
    continuer=false;
    captureThread.join();
    capture2(Samples);
    ShutdownCapture();
    ALint  NbQueued;
    ALuint Buffer;
    alSourceStop(Source);
    alGetSourcei(Source, AL_BUFFERS_QUEUED, &NbQueued);
    for (ALint i = 0; i < NbQueued; ++i)
        alSourceUnqueueBuffers(Source, 1, &Buffer);
    std::cout << " ddddddd" << Samples.size() << std::endl;
	short min = MIN_ALSHORT_VAL;
	unsigned int max = MAX_ALSHORT_VAL;
	std::cout << min << " " << max << std::endl;
	std::cout << min << " " << max << " " << sizeof(ALshort) <<  std::endl;
    
	
	dura = std::chrono::milliseconds(1000);
    std::this_thread::sleep_for(dura);
    alBufferData(Buffers[0], Format, &Samples[0], Samples.size() * sizeof(ALushort), SampleRate);
    alSourceQueueBuffers(Source, 1, &Buffers[0]);
    alSourcePlay(Source);
    do{
        alGetSourcei(Source, AL_SOURCE_STATE, &Status);
    }
    while (Status == AL_PLAYING);

    alGetSourcei(Source, AL_BUFFERS_QUEUED, &NbQueued);
    for (ALint i = 0; i < NbQueued; ++i)
        alSourceUnqueueBuffers(Source, 1, &Buffer);
    alSourcei(Source, AL_BUFFER, 0);
    alDeleteBuffers(2, Buffers);
    alDeleteSources(1, &Source);
    shutdownOpenAL();
}

















int test1(){
    initOpenAL();

    std::vector<ALshort> Samples;
    InitCapture();
    alcCaptureStart(CaptureDevice);
    while ( Samples.size()/SampleRate < 5)
    {
       ALCint SamplesAvailable;
        alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
        if (SamplesAvailable > 0)
        {
            std::size_t Start = Samples.size();
            std::cout << Start << std::endl;
            Samples.resize(Start + SamplesAvailable);
            alcCaptureSamples(CaptureDevice, &Samples[Start], SamplesAvailable);
        }
    }
    alcCaptureStop(CaptureDevice);

    ALCint SamplesAvailable;
    alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &SamplesAvailable);
    if (SamplesAvailable > 0)
    {
        std::size_t Start = Samples.size();
        Samples.resize(Start + SamplesAvailable);
        alcCaptureSamples(CaptureDevice, &Samples[Start], SamplesAvailable);
    }
    std::cout << "fin capture" << std::endl;
    ShutdownCapture();
    SaveSound("test.wav",Samples);


    ALuint Source;
    alGenSources(1, &Source);
    ALuint Buffer;
    alGenBuffers(1, &Buffer);
    alBufferData(Buffer, Format, &Samples[0], Samples.size() * sizeof(ALushort), SampleRate);

    alSourceQueueBuffers(Source, 1, &Buffer);
    alSourcePlay(Source);
    ALint Status;
    do{
        alGetSourcei(Source, AL_SOURCE_STATE, &Status);
    }
    while (Status == AL_PLAYING);

    alSourceUnqueueBuffers(Source, 1, &Buffer);
    Buffer = loadSound("test.wav",Buffer);
    alSourceQueueBuffers(Source, 1, &Buffer);
    alSourcePlay(Source);
    do{
        alGetSourcei(Source, AL_SOURCE_STATE, &Status);
    }
    while (Status == AL_PLAYING);


    alSourceUnqueueBuffers(Source, 1, &Buffer);
    alDeleteBuffers(1, &Buffer);
    alSourcei(Source, AL_BUFFER, 0);
    alDeleteSources(1, &Source);

    shutdownOpenAL();
    return EXIT_SUCCESS;
}
//*/
/*
initOpenAL();

// Création des deux tampons OpenAL que nous allons utiliser pour la lecture en flux
ALuint Buffers[2];
alGenBuffers(2, Buffers);

ALuint Source;
// Création de la source qui jouera le son
alGenSources(1, &Source);

InitCapture();

alcCaptureStart(CaptureDevice);
// Remplissage des tampons avec le début du son à jouer
ReadMicroCapture(Buffers[0], 44100);
ReadMicroCapture(Buffers[1], 44100);

// On ajoute les 2 tampons à la file de la source
alSourceQueueBuffers(Source, 2, Buffers);
// On peut commencer la lecture
alSourcePlay(Source);

ALint Status;
do
{
    // On récupère le nombre de tampons qui ont été traités (ie. qui sont prêts à être réutilisés)
    ALint NbProcessed;
    alGetSourcei(Source, AL_BUFFERS_PROCESSED, &NbProcessed);

    // On les re-remplit et on les réinjecte dans la file
    for (ALint i = 0; i < NbProcessed; ++i)
    {
        // On sort le tampon de la file
        ALuint Buffer;
        alSourceUnqueueBuffers(Source, 1, &Buffer);

        // On le remplit avec les données du fichier
        ReadMicroCapture(Buffer, 44100);

        // On le réinjecte dans la file
        alSourceQueueBuffers(Source, 1, &Buffer);
    }

    // On récupère l'état de la source
    alGetSourcei(Source, AL_SOURCE_STATE, &Status);
}
while (Status == AL_PLAYING);

    alcCaptureStop(CaptureDevice);
        ALuint Buffer;
        ReadMicroCapture(Buffer, 44100);
// On purge la file de tampons de la source
ALint  NbQueued;
//ALuint Buffer;
alGetSourcei(Source, AL_BUFFERS_QUEUED, &NbQueued);
for (ALint i = 0; i < NbQueued; ++i)
    alSourceUnqueueBuffers(Source, 1, &Buffer);
alSourcei(Source, AL_BUFFER, 0);

// On détruit les deux tampons
alDeleteBuffers(2, Buffers);

// On détruit la source
alDeleteSources(1, &Source);

//*/
//    inputReaderThread.join();
//}
/*


std::thread inputReaderThread;
bool Playing

readKeyboard();
inputReaderThread(&System::readkeyboard,this)

~Game
...inputReaderThread.join()



*/
