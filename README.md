# COMP3004FinalProject
By Eric Desrosiers - 101265942, Michael Roy - 101260953, Shiven Sharma - 101272855, Adam Tremblay - 101264116


## FILES INFO:
- IMPORT AND INSTALL QT CHARTS BEFORE RUNNING. RUN THE FOLLOWING COMMAND IN CMD --> sudo apt install libqt5charts5 libqt5charts5-dev

Header Files:
- app.h
- battery.h
- device.h
- mainwindow.h
- scan.h
- user.h

Source Files:
- app.cpp
- battery.cpp
- device.cpp
- mainwindow.cpp
- scan.cpp
- user.cpp

Forms:
- mainwindow.ui

Resources:
- imageResources.qrc

Other Files:
- resources folder (images)
- README.md

#### Sequence Scenarios:
Main Success Scenario --> RaDoTech_Sequence.png

Battery Critically Low --> RaDoTech_OutOfBattery_Sequence.png

Loss Of Connection during Treatment --> RaDoTech_Interrupt_Sequence.png

### State Diagrams:
Device + App State Diagrams --> RaDoTech_StateDiagrams.png

### Class Diagram:

### Use Cases + Traceability Matrix + Design Discussion
TEAM31_Submission.docx

## Teamwork (ALL TEAMMATES CONTRIBUTED):
	- All Use cases
	- Class ideas and Class design
	- Background Logic and Algorithms ideas
	- Organized Meetings to discuss ideas
	- UI design ideas
	- Code design and logic flow ideas
	- Research for RaDoTech for accurate recreation
 	- Design Discussion PDF

### Michael:
	- Sequence Diagrams
		- Main Sequence Scenario
		- Out of Battery/Shutdown Sequence Scenario
		- Measure Interrupt Sequence Scenario
	- UI code logic, including widget switching for the different screens and which buttons are enabled at a given time
	- UI design, designed base UI and major changes, battery design, and UI Layout and fonts, colour, etc
	- All Scan history code to access the user scan history and be able to click through
	- Signals for important UI updates such as on/off device, battery updating, on/off skin touch, current point number, and idea to add images
	- All Bug fixes to the battery and scan.
### Eric:
	- Traceability Matrix
 	- logic & implementation for the device and how it generates the data points
  	- logic & implementation for measuring and calculating the measuring graph points
   	- implementation for calculating the points for the bar/circle graph
    	- implemented the basics for the battery such as the on/off triggering the depletetion and connecting it to the device
     	- implemented graceful shutdown for if the simulation is mid scan

### Adam:
	- State Diagrams (device and app)
 	- Contributed to the design discussion pdf by cleaning up use cases, adding design discussions, and reviewing existing design discussions
 	- All functionality and UI design of the profiles page, including adding, removing, and updating user profiles. 
  	- All functionality and UI design of the home page, including the graph visual and corresponding buttons.
   	- Included all the scan point images to give the user a visual of where to scan
    	- Debugged and cleaned up a lot of the main UI functionality, filled in the holes where necessary
