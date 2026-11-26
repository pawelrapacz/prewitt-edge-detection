#
# Project: Prewitt edge detection
# 
# The Prewitt algorithm detects edges in images by calculating
# intensity changes in horizontal and vertical directions using
# two convolution kernels.
# 
# 2026/2027 sem. 5
# Author: Paweł Rapacz
# 
# Version: 1.0
# 
# Version history:
# 1.0 - Initial version of the application.
#      Implemented Prewitt edge detection in C and x86-64 ASM.
#      Created GUI app and benchmark with core functionality.
#

import cv2
import numpy as np
import math
import pathlib
import argparse


# perform the reference prewitt edge detection for other implementations
def prewitt(image, amount = 1.0):
    amount = np.float32(amount)

    kernel_gx = np.array([[ 1,  0, -1],
                          [ 1,  0, -1],
                          [ 1,  0, -1]], dtype=np.int32)

    kernel_gy = np.array([[ 1,  1,  1],
                          [ 0,  0,  0],
                          [-1, -1, -1]], dtype=np.int32)

    # detect vertically and horizontally
    gx = cv2.filter2D(image, cv2.CV_32F, kernel_gx).astype(np.int32)
    gy = cv2.filter2D(image, cv2.CV_32F, kernel_gy).astype(np.int32)

    height, width = gx.shape[:2]
    result = np.ndarray(gx.shape, np.uint8)

    # calculate result magnitude
    for i in range(height):
        for j in range(width):
            pgx = gx[i, j]
            pgy = gy[i, j]

            Rmag = math.sqrt(pgx[0] * pgx[0] + pgy[0] * pgy[0]) * amount
            Gmag = math.sqrt(pgx[1] * pgx[1] + pgy[1] * pgy[1]) * amount
            Bmag = math.sqrt(pgx[2] * pgx[2] + pgy[2] * pgy[2]) * amount

            result[i, j] = [
                np.uint8(min(255, Rmag)),
                np.uint8(min(255, Gmag)),
                np.uint8(min(255, Bmag)),
            ]
    
    # make borders black
    result[[0, -1], :] = [0, 0, 0]
    result[:, [0, -1]] = [0, 0, 0]

    return result


parser = argparse.ArgumentParser()
parser.add_argument("--input-dir", required=True)
parser.add_argument("--expect-dir", required=True)
parser.add_argument("--amount", required=True)

args = parser.parse_args()

ext = "png"
inputdir = pathlib.Path(args.input_dir)
expectdir = pathlib.Path(args.expect_dir)
amount = np.double(args.amount)


if not inputdir.is_dir():
    print("Error: Input directory not a directory or does not exist!")
    exit(1)

if len(list(inputdir.glob(f"*.{ext}"))) == 0:
    print(f"Input directory has no .{ext} files!")
    exit(1)

expectdir.mkdir(parents=True, exist_ok=True)

for path in inputdir.glob(f"*.{ext}"):
    if path.is_file():
        image = cv2.imread(str(path), cv2.IMREAD_COLOR)
        
        print("processing image: " + str(path), flush=True)
        processed = prewitt(image, amount)

        cv2.imwrite(str(expectdir / path.name), processed)

print("finished")
