# Mercy
Mercy is an audio plugin created using the JUCE framework. It is intended to be used as an example
to learn the JUCE framework. Mercy covers everything from UI building to JUCE's dsp library. 

The Mercy plugin consists of a high pass filter, low pass filter, Q factor controller, gain controller and
a bypass button. 

![](https://i.imgur.com/WQXUstg.png)

To learn from Mercy you should have an understanding of basic C++ and knowledge of how object orientated
and event driven programming works. Mercy does use some more advanced topics of C++ such as unique pointers.
It would also help to have some basic digital signal processing knowledge.

# Topics Covered
* <h3>JUCE DSP modules</h3>
JUCE provides multiple classes capable of providing basic Digital Signal Processing. For a full list of all modules [click here](https://docs.juce.com/master/group__juce__dsp-processors.html)
* <h3>The Component Class</h3>
The fundamental class JUCE provides for rendering objects to the screen. In summary, anything that is rendered to the GUI is a component. For a list of all JUCE components [click here](https://docs.juce.com/master/classComponent.html#details)(see 'inherited by' at the top).
* <h3>Building Custom Components</h3>
You are not limited to just JUCE's components, but you are able to create your own using inheritance. In this plugin, we create the 'dbLevelMeterComponent' capable of providing visual feedback on the gain of the signal passing through the plugin chain. This component was created using a YouTube tutorial I owe credit to [Akash Murthy](https://github.com/Thrifleganger). For that tutorial [click here]((https://youtu.be/ILMdPjFQ9ps).
* <h3>The Graphics Class + LookAndFeel Methods</h3>
JUCE provides many graphics options for drawing components. Most of the time you will be using these when overriding LookAndFeel methods, which are used to draw the basic JUCE components. It is important to have a decent grasp of basic trigonometry as well as both degrees and radians. For a full list of important topics to know please see the [MercyLookAndFeel.cpp file](https://github.com/kainbryanjones/Mercy/blob/main/Source/MercyLookAndFeel.cpp)
* <h3>The Rectangle Class</h3>
Rectangles are used to position components on the screen. They are useful because they can be used to measure distances and find points without knowing the exact dimensions of the plugin/application. For example, if we wanted a component to have a width a third of the total width of the plugin and a height half that of the plugin then we can simply create a rectangle with width 'getWidth() / 3.f' and height 'getHeight() / 2.f'. 
* <h3>Listeners and Broadcasters</h3>
Listeners and Broadcasters provide [event-driven functionality](https://simple.wikipedia.org/wiki/Event-driven_programming). They are key to responding to user input with functions such as mouseEnter and sliderValueChanged.
* <h3>The AudioProcessorValueTreeState Class</h3>
JUCE provides a class known as [ValueTree](https://docs.juce.com/master/classValueTree.html#details). It is a powerful tree structure that can be used to hold free-form data, and which can handle its own undo and redo behaviour. The AudioProcessorValueTreeState class contains a ValueTree that is used to manage an AudioProcessor's entire state. Through the use of classes such as [SliderAttachment](https://docs.juce.com/master/classAudioProcessorValueTreeState_1_1SliderAttachment.html) we can maintain a connection between the widget and the parameter.
* <h3>The LinearSmoothedValue Class</h3>
In audio is important to have smoothed values which is a value that doesn't immediately change with user input, but rather 'smooths' value over a given time(usually between 0.1s and 0.5s) after user input. This is important to avoid clicks and pops which can be the result of sudden waveform changes.
* <h3>Saving/Loading plugin state</h3>
Most DAWs provide plugin presets as well as persistent data which will need to be loaded back up when a plugin is reopened. In JUCE this is done using XML, which is a very simple markup language. If you don't know XML it takes no more than ten minutes to get to grips with the basics. W3Schools.org has an excellent tutorial on this. [Click here](https://www.w3schools.com/xml/) for that tutorial.
* <h3>Using Git With JUCE</h3>
This is not a git tutorial, but a few pointers on how to create a git repository and what to add to .gitignore for JUCE projects.

# Tutorials
Mercy will be accompanied by a series of tutorials on my band's blog. These will be published by the end of January and will be linked
here when available.

# Footnote
If you feel like you could improve Mercy or I have made an error somewhere then feel free to contact
me at <contact.kainbryanjones@gmail.com>
