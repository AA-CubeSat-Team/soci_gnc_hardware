clear all; close all; clc

t_send_gyro = 1/25;
t_receive_gyro = t_send_gyro;
magnitude_gyro = 250;

t_send_mag = 1/15;
t_receive_mag = t_send_mag;
magnitude_mag = 100;

t_send_phd = 0.061;
t_receive_phd = t_send_phd;
magnitude_phd = 100;

t_send_sun = 0.02;
t_receive_sun = t_send_sun;
magnitude_sun_angle = 60;
magnitude_sun_detection = 50;
bias_sun_detection = 50;

