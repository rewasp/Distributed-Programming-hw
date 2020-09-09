#!/bin/bash

mpirun --hostfile my-host -np 5 prefix_scan2 | sort
