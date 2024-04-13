clear 
clc
%% Main 
% Load the audio file
load('fix_me.mat');

% Extract the signal and sampling frequency
signal = mixed_noise; 
fs = 8000; 

% Plot the original signal in time domain and spectrogram
figure()
subplot(211), plot(1:length(mixed_noise), mixed_noise);
title('Original Signal');
subplot(212), spectrogram(mixed_noise, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram of Original Signal');

%% Remove the fire alarm

% Apply bandstop filters to remove fire alarm frequencies
y1 = bandstop(signal, [3250 3450], fs);
y2 = bandstop(y1, [1450 1550], fs);

% Plot the signal without fire alarm in time domain and spectrogram
figure()
subplot(211), plot(1:length(mixed_noise), y2);
title('Signal without Fire Alarm');
subplot(212), spectrogram(y2, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram without Fire Alarm');

%% Remove the ringtone 

% Load the ringtone
load('ringtone.mat')
%sound(ringtone_res, fs);

% Create a zero-padded ringtone and subtract it from the signal
my_ring = zeros(1, length(mixed_noise));
my_ring(4 * fs : 6 * fs - 1) = ringtone_res;
without_ring = y2 - my_ring;

% Plot the signal without ringtone in time domain and spectrogram
figure()
subplot(211), plot(1:length(mixed_noise), without_ring);
title('Signal without Ringtone');
subplot(212), spectrogram(without_ring, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram without Ringtone');

%% Remove AC noise

% Load and resample the AC noise
AC_noise = audioread("C:\Users\surak\Documents\MATLAB\AC.wav");
AC_resample = resample(AC_noise, fs, 48000);

% Adjust the length of the resampled AC noise and subtract from the signal
offset = length(mixed_noise) - length(AC_resample);
AC_res = [AC_resample; AC_resample(1:offset)];
without_ac = without_ring' - AC_res;

% Plot the signal without AC noise in time domain and spectrogram
figure()
subplot(211), plot(1:length(mixed_noise), without_ac);
title('Signal without AC Noise');
subplot(212), spectrogram(without_ac, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram without AC Noise');

%% Amplify the specified part

% Amplify a specific part of the signal
without_ac(6 * fs : end) = 1.5 * without_ac(6 * fs:end);
amplified = without_ac;

% Play the amplified signal 
sound(amplified, fs)

% Save the amplified signal to a WAV file:
amplified_filename = 'amplified_signal.wav';
audiowrite(amplified_filename, amplified, fs);

%plot the amplified signal in time domain and spectrogram
figure()
subplot(211), plot(1:length(mixed_noise), amplified);
title('Amplified Signal');
subplot(212), spectrogram(amplified, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram of Amplified Signal');

%% comparing the intial mixed noise and resulting clean noise

figure()

subplot(2, 2, 1), plot(1:length(mixed_noise), mixed_noise);
title('Original Signal');

subplot(2, 2, 2), spectrogram(mixed_noise, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram of Original Signal');

subplot(2, 2, 3), plot(1:length(mixed_noise), amplified);
title('Clean Signal');

subplot(2, 2, 4), spectrogram(amplified, hamming(1024), 512, 1024, fs, 'yaxis');
title('Spectrogram of Clean Signal');
