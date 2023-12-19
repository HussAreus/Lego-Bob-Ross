import cv2
import numpy as np

# Read the image
image = cv2.imread('LegoBobRoss\pictures\hoouse.png')

# Convert the image to grayscale
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Apply edge detection (you can use any edge detection method here)
edges = cv2.Canny(gray, 50, 150)

# Perform Hough line transform
lines = cv2.HoughLinesP(edges, 1, np.pi/180, threshold=40, minLineLength=30, maxLineGap=5)

# Create a copy of the original image
image_with_lines = np.copy(image)

line_coordinates = []

# Filter similar lines
existing_lines = None
tolerance = 11
filtered_lines = []

# Draw the lines on the image and collect coordinates
for line in lines:
    x1, y1, x2, y2 = line[0]
    #cv2.line(image_with_lines, (x1, y1), (x2, y2), (0, 255, 0), 2)
    line_coordinates.append(((x1, y1, x2, y2)))

#Filter overlaping lines
for line in line_coordinates:
    if existing_lines is None:
        existing_lines = [line]
        continue
    for existing_line in existing_lines:
        if abs(line[0] - existing_line[0]) < tolerance and abs(line[1] - existing_line[1]) < tolerance and (line[2] - existing_line[2]) < tolerance and abs(line[3] - existing_line[3]) < tolerance:
            cv2.line(image_with_lines, (line[0], line[1]), (line[2], line[3]), (0, 255, 0), 2)
            filtered_lines.append(line)
            break
    else:
        existing_lines.append(line)

line_counter = 1  # Initialize line counter

for line in filtered_lines:
    mid_point = ((line[0]+line[2])//2, (line[1]+line[3])//2)  # Calculate midpoint of the line
    cv2.putText(image_with_lines, str(line_counter), mid_point, cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)
    line_counter += 1  # Increment line counter

# Display the original and the image with lines
#cv2.imshow('Original Image', gray)
cv2.imshow('Image with Lines', image_with_lines)

# Print the filtered lines
print(filtered_lines)

cv2.waitKey(0)
cv2.destroyAllWindows()
