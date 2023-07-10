add_library(
        msfl2D
        STATIC

        Shape.cpp Shape.hpp
        Vec2D.cpp Vec2D.hpp
        Segment.cpp Segment.hpp
        Line.cpp Line.hpp
        ConvexPolygon.cpp ConvexPolygon.hpp
        World.cpp World.hpp
        MsflExceptions.cpp MsflExceptions.hpp
        Body.cpp Body.hpp CollisionDetector.cpp CollisionDetector.hpp LineSegment.cpp LineSegment.hpp CollisionResolver.cpp CollisionResolver.hpp)