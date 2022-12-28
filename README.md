![alt text](https://assets.juce.com/juce/JUCE_banner_github.png "JUCE")

JUCE is an open-source cross-platform C++ application framework for creating high quality
desktop and mobile applications, including VST, VST3, AU, AUv3, AAX and LV2 audio plug-ins
and plug-in hosts. JUCE can be easily integrated with existing projects via CMake, or can
be used as a project generation tool via the [Projucer](https://juce.com/discover/projucer),
which supports exporting projects for Xcode (macOS and iOS), Visual Studio, Android Studio,
Code::Blocks and Linux Makefiles as well as containing a source code editor.

# Disclaimer
Mercy is currently a work in progress and I intend to have it finish by early January 2023. I will
remove this disclaimer once I am happy that the project is the best it can be.

# Mercy
Mercy is an audio plugin created using the JUCE framework. It is intended to be used as an example
to learn the JUCE framework. Mercy covers everything from UI building to JUCE's dsp library. 

The Mercy plugin consists of a high pass filter, low pass filter, Q factor controller, gain controller and
a bypass button. 

To learn from Mercy you should have an understanding of basic C++ and knowledge of how object orientated
programming works. Mercy does use some more advanced topics of C++ such as unique pointers.
It would also help to have some basic digital signal processing knowledge.

# Topics Covered
* JUCE DSP modules
* The Component Class
* Building Custom Components
* The Graphics Class
* The Rectangle Class
* Listeners and Broadcasters
* The AudioProcessorValueTreeState Class
* The LinearSmoothedValue Class
* The LookAndFeel Class
* Using Git With JUCE

# Tutorials
Mercy will be accompanied by a series of tutorials on my band's blog

# Footnote
If you feel like you could improve Mercy or I have made an error somewhere then feel free to contact
me at <contact.kainbryanjones@gmail.com>