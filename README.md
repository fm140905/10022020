# Pulse processing program based on the ROOT framework


- Writes a parser to read the data saved using CoMPASS .
- Implements the CFD algorithm based on interpolation.
- Finds the time differences between the coincident events from two channels.
- Draws the histogram and fit.

## Parser

- Make sure you have installed ROOT and your OS is linux.
- Download the `input.txt` file and `main` file in `\bin` folder. Put them in the same directory.
- Create a folder named `output` in your current directory.
- Adjust the parameters in `input.txt`.
    - `Directory`, `Floders` and `Channels` together specify the file path;
    - Use `MaxNumPulses` to specify the numer of pulses you want to process. This can be set small when you want to take a look at the data but don't want to spend too much time on processing.
    - For data generated by CoMPASS, you need to change the `NSamples`, `DynamicRange`, `Resolution` and `Delt`.

## Pulse integral distribution (PID) plot
- In `input.txt`, make sure `SavePI` is 1 in line140 if you want to save the integrals.
    - A file named `PI_CH` should be generated in `output` folder, with a list of pulse integrals recorded. You can use Matlab or other tools to draw a histogram from it, which is the desired pulse integral distribution.
- In `input.txt`, make sure `PID` is 1 in line142 if you want to plot the PID in the program.
    - Some options are available, such as integral range (`Pre`, `Post`), plot range (`PImin`, `PImax`) and number of bins.

## Run
- Sometimes the file is too lagre to be processed at one time (a few GB). In this case, you can split the file into several parts and process them seperately. Or you can use a small `MaxNumPulses`.
- Open a terminal, type `./main` to run.
- Use `Ctrl`+ `c` to quit.

## Use
If you use part of this software, please cite the following paper
Fang, M., Bartholomew, N., Di Fulvio, A. Positron annihilation lifetime spectroscopy using fast scintillators and digital electronics
(2019) Nuclear Instruments and Methods in Physics Research, Section A: Accelerators, Spectrometers, Detectors and Associated Equipment, 943, art. no. 162507.