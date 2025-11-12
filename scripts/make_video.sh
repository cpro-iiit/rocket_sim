#!/bin/bash
# ==============================================================================
# make_video.sh - Generate Video from BMP Frames
# ==============================================================================
# This script converts a sequence of BMP frames from the N-body simulation
# into a video file using ffmpeg.
#
# Requirements:
#   - ffmpeg must be installed on your system
#   - BMP frames must be in the current directory or specified directory
#   - Frames should be named frame_0000.bmp, frame_0001.bmp, etc.
#
# Usage:
#   ./scripts/make_video.sh [options]
#
# Options:
#   -i DIR      Input directory containing BMP frames (default: current directory)
#   -o FILE     Output video filename (default: simulation.mp4)
#   -f FPS      Frame rate (frames per second) (default: 30)
#   -q QUALITY  Video quality: low, medium, high (default: high)
#   -c CODEC    Video codec: h264, h265, vp9 (default: h264)
#   -h          Show this help message
#
# Examples:
#   ./scripts/make_video.sh
#   ./scripts/make_video.sh -i output -o animation.mp4
#   ./scripts/make_video.sh -f 60 -q high
#   ./scripts/make_video.sh -c h265 -o orbit.mp4
#
# ==============================================================================

# ==============================================================================
# DEFAULT CONFIGURATION
# ==============================================================================

# Default input directory (where BMP frames are located)
INPUT_DIR="."

# Default output filename
OUTPUT_FILE="simulation.mp4"

# Default frame rate (frames per second)
FRAME_RATE=30

# Default video quality
QUALITY="high"

# Default video codec
CODEC="h264"

# Frame pattern (how frames are named)
FRAME_PATTERN="frame_%04d.bmp"

# ==============================================================================
# COLOR CODES FOR OUTPUT
# ==============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# ==============================================================================
# FUNCTIONS
# ==============================================================================

# Print colored message
print_message() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Print error message and exit
error_exit() {
    print_message "$RED" "ERROR: $1"
    exit 1
}

# Show help message
show_help() {
    cat << EOF

========================================
Video Generation Script
========================================

Generate video files from BMP frame sequences.

USAGE:
    $0 [options]

OPTIONS:
    -i DIR      Input directory with BMP frames (default: .)
    -o FILE     Output video filename (default: simulation.mp4)
    -f FPS      Frame rate (default: 30)
    -q QUALITY  Video quality: low, medium, high (default: high)
    -c CODEC    Video codec: h264, h265, vp9 (default: h264)
    -h          Show this help message

EXAMPLES:
    # Basic usage (uses defaults)
    $0

    # Specify input directory and output file
    $0 -i output -o orbit_animation.mp4

    # High quality 60 FPS video
    $0 -f 60 -q high

    # Use H.265 codec for better compression
    $0 -c h265 -o simulation_compressed.mp4

    # Low quality for quick preview
    $0 -q low -o preview.mp4

VIDEO QUALITY:
    low     - Fast encoding, larger file size, lower quality
    medium  - Balanced encoding speed and quality
    high    - Slow encoding, smaller file size, best quality

VIDEO CODECS:
    h264    - H.264/AVC - Most compatible, good quality
    h265    - H.265/HEVC - Better compression, newer codec
    vp9     - VP9 - Open codec, good for web

NOTES:
    - ffmpeg must be installed on your system
    - Frames should be named frame_0000.bmp, frame_0001.bmp, etc.
    - Higher FPS makes smoother video but larger file size
    - High quality takes longer to encode but produces smaller files

========================================

EOF
    exit 0
}

# Check if ffmpeg is installed
check_ffmpeg() {
    if ! command -v ffmpeg &> /dev/null; then
        error_exit "ffmpeg is not installed. Please install ffmpeg first.

On Ubuntu/Debian:
    sudo apt-get install ffmpeg

On macOS:
    brew install ffmpeg

On Windows:
    Download from https://ffmpeg.org/download.html"
    fi
    
    print_message "$GREEN" "✓ ffmpeg found: $(ffmpeg -version | head -n1)"
}

# Count BMP frames in directory
count_frames() {
    local count=$(ls -1 "$INPUT_DIR"/frame_*.bmp 2>/dev/null | wc -l)
    echo $count
}

# Get quality settings based on quality level
get_quality_settings() {
    case $QUALITY in
        low)
            echo "-preset ultrafast -crf 28"
            ;;
        medium)
            echo "-preset medium -crf 23"
            ;;
        high)
            echo "-preset slow -crf 18"
            ;;
        *)
            error_exit "Invalid quality: $QUALITY (use: low, medium, high)"
            ;;
    esac
}

# Get codec settings
get_codec_settings() {
    case $CODEC in
        h264)
            echo "-c:v libx264"
            ;;
        h265)
            echo "-c:v libx265"
            ;;
        vp9)
            echo "-c:v libvpx-vp9"
            ;;
        *)
            error_exit "Invalid codec: $CODEC (use: h264, h265, vp9)"
            ;;
    esac
}

# Generate the video
generate_video() {
    print_message "$CYAN" "\n=========================================="
    print_message "$CYAN" "Starting Video Generation"
    print_message "$CYAN" "=========================================="
    
    # Display settings
    print_message "$BLUE" "\nSettings:"
    echo "  Input directory:  $INPUT_DIR"
    echo "  Output file:      $OUTPUT_FILE"
    echo "  Frame rate:       $FRAME_RATE FPS"
    echo "  Quality:          $QUALITY"
    echo "  Codec:            $CODEC"
    echo "  Frame pattern:    $FRAME_PATTERN"
    
    # Check for frames
    local num_frames=$(count_frames)
    if [ $num_frames -eq 0 ]; then
        error_exit "No BMP frames found in $INPUT_DIR/
Please run the simulation first to generate frames."
    fi
    
    print_message "$GREEN" "\n✓ Found $num_frames frames"
    
    # Calculate video duration
    local duration=$(echo "scale=2; $num_frames / $FRAME_RATE" | bc)
    print_message "$BLUE" "  Estimated duration: ${duration}s"
    
    # Get encoding settings
    local quality_settings=$(get_quality_settings)
    local codec_settings=$(get_codec_settings)
    
    # Build ffmpeg command
    local ffmpeg_cmd="ffmpeg -y -framerate $FRAME_RATE"
    ffmpeg_cmd="$ffmpeg_cmd -i $INPUT_DIR/$FRAME_PATTERN"
    ffmpeg_cmd="$ffmpeg_cmd $codec_settings $quality_settings"
    ffmpeg_cmd="$ffmpeg_cmd -pix_fmt yuv420p"
    ffmpeg_cmd="$ffmpeg_cmd $OUTPUT_FILE"
    
    print_message "$YELLOW" "\n⚙ Encoding video..."
    print_message "$BLUE" "  Command: $ffmpeg_cmd"
    echo ""
    
    # Run ffmpeg
    if eval $ffmpeg_cmd; then
        print_message "$GREEN" "\n=========================================="
        print_message "$GREEN" "✓ Video created successfully!"
        print_message "$GREEN" "=========================================="
        
        # Show file information
        if [ -f "$OUTPUT_FILE" ]; then
            local file_size=$(du -h "$OUTPUT_FILE" | cut -f1)
            print_message "$BLUE" "\nOutput file:"
            echo "  Filename: $OUTPUT_FILE"
            echo "  Size:     $file_size"
            echo "  Frames:   $num_frames"
            echo "  Duration: ${duration}s"
            echo "  FPS:      $FRAME_RATE"
            
            # Try to get video info with ffprobe if available
            if command -v ffprobe &> /dev/null; then
                local resolution=$(ffprobe -v error -select_streams v:0 \
                    -show_entries stream=width,height -of csv=s=x:p=0 "$OUTPUT_FILE" 2>/dev/null)
                if [ ! -z "$resolution" ]; then
                    echo "  Resolution: $resolution"
                fi
            fi
        fi
        
        print_message "$CYAN" "\nYou can now play the video with:"
        print_message "$BLUE" "  vlc $OUTPUT_FILE"
        print_message "$BLUE" "  ffplay $OUTPUT_FILE"
        print_message "$BLUE" "  mpv $OUTPUT_FILE"
        echo ""
    else
        error_exit "Video encoding failed. Check the error messages above."
    fi
}

# ==============================================================================
# PARSE COMMAND LINE ARGUMENTS
# ==============================================================================

while getopts "i:o:f:q:c:h" opt; do
    case $opt in
        i)
            INPUT_DIR="$OPTARG"
            ;;
        o)
            OUTPUT_FILE="$OPTARG"
            ;;
        f)
            FRAME_RATE="$OPTARG"
            ;;
        q)
            QUALITY="$OPTARG"
            ;;
        c)
            CODEC="$OPTARG"
            ;;
        h)
            show_help
            ;;
        \?)
            error_exit "Invalid option: -$OPTARG. Use -h for help."
            ;;
    esac
done

# ==============================================================================
# MAIN EXECUTION
# ==============================================================================

# Check if ffmpeg is installed
check_ffmpeg

# Validate input directory
if [ ! -d "$INPUT_DIR" ]; then
    error_exit "Input directory does not exist: $INPUT_DIR"
fi

# Generate the video
generate_video

exit 0

# ==============================================================================
# END OF SCRIPT
# ==============================================================================