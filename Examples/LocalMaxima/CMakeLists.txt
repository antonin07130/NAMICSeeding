SET( NAMICSEED_EXAMPLE_SRC
  LocalMaxima
  LocalMaxima2D
  CombineSeeds
  )


FOREACH( var ${NAMICSEED_EXAMPLE_SRC} )

  ADD_EXECUTABLE( ${var} ${var} )
  TARGET_LINK_LIBRARIES( ${var}
  ITKIO
  ITKQuadEdgeMesh
  ITKCommon
  ITKBasicFilters
  ITKAlgorithms
  ITKStatistics
  ITKNumerics
  )


ENDFOREACH( var ${NAMICSEED_EXAMPLE_SRC} )
