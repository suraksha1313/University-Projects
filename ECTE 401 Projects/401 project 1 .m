% Path to the chest X-ray image folders 
imageFolderPath = "C:\Users\surak\Downloads\xray\images";
annotationsPath = "C:\Users\surak\Downloads\xray\annotations\imageannotation_ai_lung_bounding_boxes.json";

% Path for the enhanced images folder
enhancedImageFolderPath = 'C:\Users\surak\Downloads\xray\enhanced images';

% Load JSON annotations
annotateData = jsondecode(fileread(annotationsPath));

% To check if annotateData is a cell array 
if iscell(annotateData)
    annotateData = annotateData{1}; % extract the first element
end

List of all image files from imagePath
imageFiles = dir(fullfile(imageFolderPath, '*.jpg')); 

% Parameters
clipLimit = 0.8;   % Clip limit for CLAHE (from 0 to 1)
stretchlimLimits = [0.01 0.99]; % Lower and upper percentile values for stretch limits
contrastStretching = true; % Assign contrast stretching as true 

% Loop through each image and apply enhancement based on annotations
for i = 1:length(imageFiles)
    % Read the image
    imagePath = fullfile(imageFolderPath, imageFiles(i).name);
    ogImg = imread(imagePath);
    
    % Convert the image to grayscale if it's in color
    if size(ogImg, 3) == 3 % for color, it is 3 color channels
        grayImg = rgb2gray(ogImg);
    else
        grayImg = ogImg; % image is grayscale  
    end
    
    % Extract bounding box coordinates from annotations
    try
        annotations = annotateData.annotations{i}; % Access annotations for the current image index
    catch
        % If an error occurs, skip this image 
        continue; % continue to the next one
    end
    
    % Apply CLAHE to the regions inside the bounding boxes
    enhancedImg = grayImg;

    % Loop through each bounding box in annotations and apply enhancement
    for j = 1:length(annotations)
        try
            x = annotations{j}.bbox(1); % x-coordinate of the bounding box from annotations
            y = annotations{j}.bbox(2); % y-coordinate of the bounding box from annotations
            width = annotations{j}.bbox(3); % Width of the bounding box from annotations
            height = annotations{j}.bbox(4); % Height of the bounding box from annotations
            
            % Apply CLAHE to the region inside the bounding box
            r = enhancedImg(y: y + height - 1, x: x + width - 1);
            r = adapthisteq(r, 'ClipLimit', clipLimit, 'Distrubtion', 'rayleigh');
            enhancedImg(y: y + height - 1, x: x + width - 1) = r; % replaces with enhanced image
        
        catch
            % If an error occurs, skip and continue to the next one
            continue;
        end
    end
    
    % if enabled 
    if contrastStretching
        % stretch limits (percentiles)
        stretchLimits = stretchlim(enhancedImg, stretchlimLimits);
        % contrast stretching
        enhancedImg = imadjust(enhancedImg, stretchLimits, []);
    end
    
%     % Display and save the enhanced image
%     figure;
%     imshowpair(grayImg, enhancedImg, 'montage'); % side by side
%     title(['Original vs Enhanced - Image ' num2str(i)]); % displaying the title with image number
%     
% Save the output images to the "enhanced images" folder
    enhancedImgPath = fullfile(enhancedImageFolderPath, ['enhanced_' imageFiles(i).name]);
     imwrite(enhancedImg, enhancedImgPath); 
 end


while(1)
    i = input('Enter the index of the image you want to view (0 to exit): ');

    if i == 0
        break; % exit the loop if user enters 0
    end

    if i >= 1 && i <= length(imageFiles)
        % Read the original image
         imagePath = fullfile(imageFolderPath, imageFiles(i).name);
         ogImg = imread(imagePath);
        
        % Read the enhanced image
          enhancedImagePath = fullfile(enhancedImageFolderPath, ['enhanced_' imageFiles(i).name]);
          enhancedImg = imread(enhancedImagePath);
        
        % Display the original and enhanced images using imshowpair
        figure;
        imshowpair(ogImg, enhancedImg, 'montage');
        title(['Original vs Enhanced - Image ' num2str(i)]); % displaying the title with image number
    else
        disp('WRONG NUMBER!! Please enter a valid index :)');
    end
    
end


    
    