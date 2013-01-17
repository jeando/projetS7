#include"../MyAL.h"
int main()
{
	AL_Play alp;
	alp.put_sound_in_buffer("gdgd.wav");
	alp.play();
	while(alp.is_playing());
	alp.stop();
	alp.stop();
}
