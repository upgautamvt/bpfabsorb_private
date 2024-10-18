from PIL import Image

# Load the images
ops_img = Image.open('ops.png')
tps_img = Image.open('tps.png')
net_rate_img = Image.open('net_rate.png')

# Set the width and height for each image
width, height = ops_img.size

# Create a new blank image with a width equal to the sum of the widths of the individual images
# and a height equal to the maximum height of the individual images
combined_width = width
combined_height = height * 3  # Stacking images vertically
combined_img = Image.new('RGB', (combined_width, combined_height))

# Paste each image into the combined image
combined_img.paste(ops_img, (0, 0))  # Place ops_img at the top
combined_img.paste(tps_img, (0, height))  # Place tps_img below ops_img
combined_img.paste(net_rate_img, (0, height * 2))  # Place net_rate_img below tps_img

# Save the combined image
combined_img.save('combined.png')

# Optional: Display the combined image
combined_img.show()
