# DPP-ROOT Changelog

## Nov. 18, 2019
### New
- Add `SaveHeaders` for saving timestamps, energy(ch) and energy short(ch) stored in the header.
- Add `Polarity` to specify the polarity of pulse. 
- Add `Pre-trigger`. Change the definitions of `ShortGate`, `LongGate`. New definitions are the same as the ones defined in CoMPASS.
- Remove `NSamples`. Read the number of samples from the header directly.
- Use `SavePulses`, `Save bad` to specify the number of pulses to be saved.
- Remove 'Calibration'. Add `CalibrationPHD` and `CalibrationPID` to calibrate PHD and PID, respectively.