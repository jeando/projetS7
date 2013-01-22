#include"../MyAL.h"
int main(int argc, char** argv)
{
	AL_Play alp;
	if(argc<2)
		alp.put_sound_in_buffer("../gdgd.wav");
	else
		alp.put_sound_in_buffer(argv[1]);
	alp.play();
	while(alp.is_playing());
	alp.stop_playing();
	alp.stop_playing();
}
