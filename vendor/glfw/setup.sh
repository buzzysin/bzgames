HERE=$(dirname $(realpath $0))

# Download glfw if it does not exist
if [ ! -d "${HERE}/../../build/_deps/glfw" ] || [ -z "$(ls -A ${HERE}/../../build/_deps/glfw)" ]; then
  # Create the directory if it does not exist
  mkdir -p "${HERE}/../../build/_deps/glfw-3.4"

  # Download the latest version of glfw
  wget -qO- https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip >/tmp/glfw-3.4.zip

  # Unzip the file
  unzip /tmp/glfw-3.4.zip -d "/tmp"
  rm /tmp/glfw-3.4.zip

  # Move the files to the build directory
  mv /tmp/glfw-3.4 "${HERE}/../../build/_deps"

  # Remove the temporary directory
  rm -rf /tmp/glfw-3.4
fi
