import {useSpring, animated, config, useSpringRef} from 'react-spring'
import {useState, useRef, useEffect} from 'react'
import ScrollButton from './ScrollButton'

export default function InfinityScroll({ children }){

    const contentRef = useRef();
    const scrollRef = useSpringRef();
    const [width, setWidth] = useState(0);

    const [{scrollVal}, setSpring] = useSpring(() => ({
        from: {scrollVal: 0},
        config: config.molasses,
    }))

    useEffect(() => {
        if(contentRef.current){
            setWidth(contentRef.current.offsetWidth);
            setSpring.set({scrollVal: contentRef.current.offsetWidth});
        }
    }, [contentRef, setSpring]);

    const scrollHandler = async (dir = false) => {};

    return <div className="row">
        <ScrollButton direction='left' />
        <ScrollButton direction='right' />
        <animated.div className="translate-row" ref={scrollRef} scrollLeft={scrollVal}>
            <div>{children}</div>
            <div ref={contentRef}>{children}</div>
            <div>{children}</div>
        </animated.div>
    </div>

}