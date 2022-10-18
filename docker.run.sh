#!/bin/bash

docker run -it -p 5900:5900 --network host -v ${PWD}:/app --rm qt bash
