#include"../MyAL.h"
int main(int argc, char** argv)
{
	std::vector<ALshort> samples;
	AL_Capture alc(MyAL::choisir_device(),MyAL::choisir_capture_device());
	alc.start(samples);
	std::chrono::milliseconds dura(1000);
	std::this_thread::sleep_for(dura);
	alc.stop();
	if(argc<2)
		alc.save_sound("test.wav");
	else
		alc.save_sound(argv[1]);
	return EXIT_SUCCESS;

}
