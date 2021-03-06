/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiScaleHessianSmoothed3DToMembranenessMeasureImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007/04/01 21:19:46 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkNumericTraits.h"


int main(int argc, char* argv [] )
{
  if ( argc < 8 )
    {
    std::cerr << "Missing Parameters: "
              << argv[0] << std::endl
              << "inputImage outputImage"
              << " [NeighborhoodSize]" << std::endl
              << "Allow outside of foreground detection (0-1)"<< std::endl;
    return EXIT_FAILURE;
    }

  // default sigma min-max in world coordinate?
  int m_NeighbSize = (int)atof(argv[4]);

  // Define the dimension of the images
  const int Dimension = 3;

  // Declare the types of the images
  typedef unsigned char       InputPixelType;
  typedef itk::Image< InputPixelType, Dimension>  InputImageType;
  typedef float               OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension> OutputImageType;

  // input image reader
  typedef itk::ImageFileReader< InputImageType  > ImageReaderType;

  // LoG filter
  typedef itk::MedianImageFilter< InputImageType, OutputImageType >
    MedianImageFilterType;
    
  typedef itk::ImageFileWriter< OutputImageType > WriterType;


  //*************READING image
  std::cout << "reading input image" << std::endl;
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName ( argv[1] );
  reader->Update();

  //**************SIGNED SQUARE DISTANCE COMPUTING
  std::cout << "foreground distance map computing" << std::endl;
  DistanceFilterType::Pointer distanceFilter
    = DistanceFilterType::New();
  distanceFilter->SetInput(readerFore->GetOutput());
  distanceFilter->SetUseImageSpacing(true);
  distanceFilter->SetInsideIsPositive(false);
  distanceFilter->SetSquaredDistance (true);
  distanceFilter->SetBackgroundValue(itk::NumericTraits< OutputPixelType >::Zero );
  distanceFilter->Update();

  //*************MULTISCALE LOG FILTERING
  MultiScaleLoGDistanceFilterType::Pointer LoGFilter
                               = MultiScaleLoGDistanceFilterType::New();
  LoGFilter->SetInput( reader->GetOutput() ); // input image
  LoGFilter->SetDistanceMap( distanceFilter->GetOutput() ); // squared sdf
  LoGFilter->SetSigmaMin(m_SigmaMin);
  LoGFilter->SetSigmaMax(m_SigmaMax);
  LoGFilter->SetNumberOfSigmaSteps(m_NumberOfSigmaSteps);
  LoGFilter->SetComputeOutsideForeground( m_OutsideComputation );
  LoGFilter->Update();


  //**********WRITING
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput ( LoGFilter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }
  //*****************

  return EXIT_SUCCESS;
}

