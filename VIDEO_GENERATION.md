# Video Generation Guide

Complete guide for generating videos from N-body simulation frames using the provided script.

---

## 📋 Table of Contents

1. [Quick Start](#quick-start)
2. [Installation](#installation)
3. [Usage Examples](#usage-examples)
4. [Options Reference](#options-reference)
5. [Video Quality Guide](#video-quality-guide)
6. [Codec Comparison](#codec-comparison)
7. [Troubleshooting](#troubleshooting)
8. [Advanced Usage](#advanced-usage)
9. [Tips and Tricks](#tips-and-tricks)

---

## 🚀 Quick Start

### Basic Usage

```bash
# 1. Run simulation to generate frames
make run

# 2. Generate video with default settings
make video

# Or use the script directly
./scripts/make_video.sh
```

This creates `simulation.mp4` at 30 FPS with high quality using H.264 codec.

---

## 📦 Installation

### Install ffmpeg

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install ffmpeg
```

#### macOS
```bash
brew install ffmpeg
```

#### Windows
1. Download from [ffmpeg.org](https://ffmpeg.org/download.html)
2. Extract to `C:\ffmpeg`
3. Add `C:\ffmpeg\bin` to PATH

### Verify Installation
```bash
ffmpeg -version
```

### Make Script Executable
```bash
chmod +x scripts/make_video.sh
```

---

## 💡 Usage Examples

### Example 1: Basic Video Creation
```bash
# Generate default video (30 FPS, high quality, H.264)
./scripts/make_video.sh
```

**Output**: `simulation.mp4`

### Example 2: Smooth 60 FPS Video
```bash
# Create smooth high-framerate video
./scripts/make_video.sh -f 60 -q high
```

**Output**: Smooth 60 FPS video (twice as smooth as default)

### Example 3: Quick Preview
```bash
# Fast encoding for quick preview
./scripts/make_video.sh -q low -o preview.mp4
```

**Output**: `preview.mp4` (encodes quickly, larger file)

### Example 4: Small File Size
```bash
# Use H.265 for better compression
./scripts/make_video.sh -c h265 -o compressed.mp4
```

**Output**: `compressed.mp4` (50% smaller than H.264)

### Example 5: Specify Input Directory
```bash
# Use frames from output/ directory
./scripts/make_video.sh -i output -o orbit_animation.mp4
```

**Output**: `orbit_animation.mp4` from frames in `output/`

### Example 6: Professional Quality
```bash
# Maximum quality for presentations
./scripts/make_video.sh -f 60 -q high -c h264 -o presentation.mp4
```

**Output**: Highest quality video suitable for presentations

### Example 7: Web Optimization
```bash
# Optimize for web streaming
./scripts/make_video.sh -c vp9 -q medium -o web_video.webm
```

**Output**: WebM file optimized for web playback

---

## 🎛️ Options Reference

### `-i DIR` - Input Directory
Specifies where BMP frames are located.

```bash
./scripts/make_video.sh -i output
./scripts/make_video.sh -i /path/to/frames
```

**Default**: Current directory (`.`)

### `-o FILE` - Output Filename
Sets the output video filename.

```bash
./scripts/make_video.sh -o animation.mp4
./scripts/make_video.sh -o results/orbit.mp4
```

**Default**: `simulation.mp4`  
**Supported formats**: `.mp4`, `.mkv`, `.webm`, `.avi`

### `-f FPS` - Frame Rate
Sets frames per second (video smoothness).

```bash
./scripts/make_video.sh -f 24   # Cinematic
./scripts/make_video.sh -f 30   # Standard (default)
./scripts/make_video.sh -f 60   # Smooth
./scripts/make_video.sh -f 120  # Very smooth
```

**Common frame rates**:
- 24 FPS: Cinematic (film standard)
- 30 FPS: Standard video (default)
- 60 FPS: Smooth motion
- 120 FPS: Very smooth (large files)

**Default**: 30 FPS

### `-q QUALITY` - Video Quality
Controls encoding quality vs file size tradeoff.

```bash
./scripts/make_video.sh -q low     # Fast, large files
./scripts/make_video.sh -q medium  # Balanced
./scripts/make_video.sh -q high    # Slow, small files
```

| Quality | Encoding Speed | File Size | Use Case |
|---------|---------------|-----------|----------|
| low | Fast | Large | Quick previews |
| medium | Moderate | Medium | General use |
| high | Slow | Small | Final videos |

**Default**: `high`

### `-c CODEC` - Video Codec
Selects the video compression codec.

```bash
./scripts/make_video.sh -c h264   # Most compatible
./scripts/make_video.sh -c h265   # Better compression
./scripts/make_video.sh -c vp9    # Open codec
```

**Default**: `h264`

### `-h` - Help
Show help message.

```bash
./scripts/make_video.sh -h
```

---

## 📊 Video Quality Guide

### Quality Settings Explained

#### Low Quality (`-q low`)
- **CRF**: 28 (higher = lower quality)
- **Preset**: ultrafast
- **Pros**: 
  - Very fast encoding (seconds)
  - Good for quick tests
- **Cons**:
  - Larger files (~2x size)
  - Visible compression artifacts
- **Use for**: Quick previews, testing

#### Medium Quality (`-q medium`)
- **CRF**: 23
- **Preset**: medium
- **Pros**:
  - Good balance of speed and quality
  - Reasonable file sizes
- **Cons**:
  - Moderate encoding time (minutes)
- **Use for**: General purpose videos

#### High Quality (`-q high`)
- **CRF**: 18 (lower = higher quality)
- **Preset**: slow
- **Pros**:
  - Excellent visual quality
  - Smaller file sizes
  - Best for archival
- **Cons**:
  - Slow encoding (10+ minutes)
- **Use for**: Final videos, presentations, sharing

### Quality Comparison Table

| Quality | 100 frames | 500 frames | 1000 frames |
|---------|-----------|------------|-------------|
| Low | ~10 MB | ~50 MB | ~100 MB |
| Medium | ~5 MB | ~25 MB | ~50 MB |
| High | ~3 MB | ~15 MB | ~30 MB |

*Approximate file sizes for 800x800 resolution*

---

## 🎬 Codec Comparison

### H.264 (AVC) - Default
**Command**: `-c h264`

**Pros**:
- Universal compatibility (all devices)
- Hardware acceleration on most systems
- Fast encoding/decoding
- Good quality/size ratio

**Cons**:
- Not the most efficient compression
- Licensing concerns for some uses

**Best for**: 
- Sharing videos
- Maximum compatibility
- Hardware playback

### H.265 (HEVC)
**Command**: `-c h265`

**Pros**:
- 50% better compression than H.264
- Smaller file sizes
- Better quality at same bitrate
- Good for 4K content

**Cons**:
- Slower encoding
- Less compatible (needs newer devices)
- Higher CPU usage for playback

**Best for**:
- Archival storage
- Bandwidth-limited sharing
- High-resolution content

### VP9
**Command**: `-c vp9`

**Pros**:
- Open source (no licensing)
- Similar compression to H.265
- Good for web streaming
- YouTube uses VP9

**Cons**:
- Slower encoding than H.264
- Less hardware support
- Limited device compatibility

**Best for**:
- Web publishing
- Open source projects
- YouTube uploads

### Codec Comparison Table

| Feature | H.264 | H.265 | VP9 |
|---------|-------|-------|-----|
| Compatibility | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |
| Compression | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Encode Speed | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐ |
| Hardware Support | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ |
| Open Source | ❌ | ❌ | ✅ |

---

## 🔧 Troubleshooting

### Problem: "ffmpeg not found"

**Solution**:
```bash
# Install ffmpeg (see Installation section)
sudo apt-get install ffmpeg

# Verify installation
which ffmpeg
```

### Problem: "No frames found"

**Solution**:
```bash
# Check if frames exist
ls -l frame_*.bmp

# If no frames, run simulation first
make run

# Or specify correct directory
./scripts/make_video.sh -i /path/to/frames
```

### Problem: "Permission denied"

**Solution**:
```bash
# Make script executable
chmod +x scripts/make_video.sh

# Then run it
./scripts/make_video.sh
```

### Problem: "Encoding failed"

**Possible causes**:
1. Frames are corrupted
2. Not enough disk space
3. Invalid codec for output format

**Solution**:
```bash
# Check disk space
df -h

# Try different codec
./scripts/make_video.sh -c h264

# Try simpler settings
./scripts/make_video.sh -q low
```

### Problem: Video plays back slowly

**Cause**: FPS too high for system

**Solution**:
```bash
# Reduce frame rate
./scripts/make_video.sh -f 30
```

### Problem: Video is too large

**Solutions**:
```bash
# Use better codec
./scripts/make_video.sh -c h265

# Increase compression
./scripts/make_video.sh -q high

# Reduce frame rate
./scripts/make_video.sh -f 24
```

### Problem: Poor video quality

**Solutions**:
```bash
# Use high quality
./scripts/make_video.sh -q high

# Use better codec
./scripts/make_video.sh -c h265

# Increase frame rate
./scripts/make_video.sh -f 60
```

---

## 🚀 Advanced Usage

### Custom ffmpeg Parameters

Edit the script to add custom ffmpeg parameters:

```bash
# In make_video.sh, find the ffmpeg command line
# Add your custom parameters before $OUTPUT_FILE

# Example: Add constant bitrate
ffmpeg_cmd="$ffmpeg_cmd -b:v 5M"

# Example: Add audio (silent black screen)
ffmpeg_cmd="$ffmpeg_cmd -f lavfi -i anullsrc"
```

### Combining Multiple Videos

```bash
# Create video from first half of simulation
./scripts/make_video.sh -o part1.mp4

# (Manually move/rename frames)

# Create video from second half
./scripts/make_video.sh -o part2.mp4

# Combine videos
ffmpeg -i part1.mp4 -i part2.mp4 -filter_complex concat output.mp4
```

### Adding Music/Narration

```bash
# After creating video, add audio
ffmpeg -i simulation.mp4 -i music.mp3 -c copy -map 0:v:0 -map 1:a:0 final.mp4
```

### Converting to GIF

```bash
# Create GIF for web embedding
ffmpeg -i simulation.mp4 -vf "fps=15,scale=400:-1" -loop 0 animation.gif
```

### Creating Thumbnail

```bash
# Extract single frame as thumbnail
ffmpeg -i simulation.mp4 -ss 00:00:05 -vframes 1 thumbnail.jpg
```

---

## 💡 Tips and Tricks

### 1. Optimize for Different Platforms

#### For YouTube
```bash
./scripts/make_video.sh -c h264 -q high -f 60
```
- YouTube prefers H.264
- 60 FPS for smooth playback
- High quality for best results

#### For Twitter
```bash
./scripts/make_video.sh -c h264 -q medium -f 30 -o twitter.mp4
# Then compress if > 512 MB
```

#### For Email
```bash
./scripts/make_video.sh -c h265 -q medium -f 24 -o email.mp4
# Aim for < 10 MB
```

### 2. Batch Processing

Process multiple simulation runs:

```bash
#!/bin/bash
# batch_video.sh
for dir in sim_run_*; do
    ./scripts/make_video.sh -i "$dir" -o "${dir}.mp4"
done
```

### 3. Progress Monitoring

Watch encoding progress in another terminal:

```bash
# Terminal 1: Start encoding
./scripts/make_video.sh

# Terminal 2: Monitor progress
watch -n 1 'ls -lh simulation.mp4'
```

### 4. Optimal Settings by Use Case

| Use Case | FPS | Quality | Codec | Notes |
|----------|-----|---------|-------|-------|
| Quick preview | 24 | low | h264 | Fastest |
| General sharing | 30 | high | h264 | Best compatibility |
| Presentation | 60 | high | h264 | Smooth, professional |
| Archival | 60 | high | h265 | Best quality/size |
| Web streaming | 30 | medium | vp9 | Good for bandwidth |
| Scientific publication | 30 | high | h264 | Standard, compatible |

### 5. Disk Space Management

```bash
# Check frame size
du -sh frame_*.bmp

# Estimate final video size
# Rule of thumb: video ≈ frames / 50 (for high quality H.264)

# Remove frames after video creation
make video && rm frame_*.bmp
```

### 6. Quality Verification

```bash
# Play video to check quality
vlc simulation.mp4
# or
ffplay simulation.mp4
# or
mpv simulation.mp4

# Get detailed video information
ffprobe -v error -show_format -show_streams simulation.mp4
```

### 7. Frame Rate vs Simulation Steps

**Match frame rate to simulation smoothness**:

- 100 frames → 24 FPS = 4.2 seconds
- 100 frames → 30 FPS = 3.3 seconds
- 100 frames → 60 FPS = 1.7 seconds

Adjust `frames=` in `config.txt` to control video duration.

---

## 📚 Additional Resources

### ffmpeg Documentation
- [Official Documentation](https://ffmpeg.org/documentation.html)
- [H.264 Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/H.264)
- [H.265 Encoding Guide](https://trac.ffmpeg.org/wiki/Encode/H.265)

### Video Optimization
- [YouTube Upload Requirements](https://support.google.com/youtube/answer/1722171)
- [Twitter Video Specifications](https://developer.twitter.com/en/docs/twitter-api/v1/media/upload-media/uploading-media/media-best-practices)

### Tools
- VLC Media Player: [videolan.org](https://www.videolan.org/)
- HandBrake (GUI): [handbrake.fr](https://handbrake.fr/)
- ffmpeg-python: Python wrapper for ffmpeg

---

## 🎯 Quick Reference

### Common Commands

```bash
# Default video
make video

# High quality 60 FPS
./scripts/make_video.sh -f 60 -q high

# Small file size
./scripts/make_video.sh -c h265

# Quick preview
./scripts/make_video.sh -q low -o test.mp4

# Custom directory
./scripts/make_video.sh -i output -o animation.mp4
```

### File Size Estimates (800x800, 100 frames)

| Settings | Approximate Size |
|----------|------------------|
| 30 FPS, low, h264 | ~10 MB |
| 30 FPS, medium, h264 | ~5 MB |
| 30 FPS, high, h264 | ~3 MB |
| 60 FPS, high, h264 | ~6 MB |
| 30 FPS, high, h265 | ~1.5 MB |

---

## 🤝 Getting Help

If you encounter issues:

1. Check this documentation
2. Verify ffmpeg installation: `ffmpeg -version`
3. Check frame files exist: `ls frame_*.bmp`
4. Try simpler settings: `-q low -f 24`
5. Check ffmpeg output for error messages

For bug reports or feature requests, please open an issue on the project repository.

---

**Happy video making!** 🎬