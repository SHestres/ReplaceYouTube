// @refresh reset

import {useSpring, animated, config, useSpringRef} from 'react-spring'
import {useState, useRef, useEffect, useCallback} from 'react'
import ScrollButton from './ScrollButton'

const offset = window.innerWidth * 0.9;

export default function InfinityScroll({ children}){

    const contentRef = useRef();
    const scrollRef = useSpringRef();
    const [width, setWidth] = useState(0);
    const [scrollPos, setScrollPos] = useState(0);
    const [animating, setAnimating] = useState(false);

    const [{scrollVal}, setSpring] = useSpring(() => ({
        from: {scrollVal: 0},
        config: config.default,
    }))

    const onRowChange = useCallback((mutationList) => {
        let wid = contentRef.current.offsetWidth;
        setWidth(wid);
    }, [setWidth])

    useMutationObservable(contentRef.current, onRowChange);

    useEffect(() => {
        if(contentRef.current){
            setWidth(contentRef.current.offsetWidth);
            setSpring.set({scrollVal: contentRef.current.offsetWidth});
        }
    }, [contentRef, setSpring]);

    //dir of true means left
    const scrollHandler = async (dir = false) => {
        setAnimating(true);
        let val = scrollPos;
        let computedScrollVal = (val % width) + width;
        await setSpring.set({scrollVal: computedScrollVal});
        await setSpring.start({ scrollVal: dir ? computedScrollVal - offset : computedScrollVal + offset, 
                                onRest(){setAnimating(false)}})
    };

    const handleManualScroll = (event) => {
        let val = event.currentTarget.scrollLeft;
        let computedScrollVal = (val % width) + width;
        setScrollPos(computedScrollVal);
        if(!animating) //When scrolling manually
            setSpring.set({scrollVal: computedScrollVal})
    }

    const checkWidth = () => {
        if(contentRef.current)
            return contentRef.current.offsetWidth > window.innerWidth;
        else
            return false;
    }

    return <div className="row">
        {checkWidth() ? <>
            <ScrollButton direction="left" onClick={scrollHandler.bind(null, true)}/>
            <ScrollButton direction='right' onClick={scrollHandler.bind(null, false)}/>
        </> : null}
        <animated.div className="translate-row" ref={scrollRef} scrollLeft={scrollVal} onScroll={handleManualScroll}>
                {checkWidth() ? <div>{children}</div> : null}
                <div ref={contentRef}>{children}</div>
                {checkWidth() ? <div>{children}</div> : null}
        </animated.div>
    </div>

}


const DEFAULT_OPTIONS = {
    config: { attributes: true, childList: true, subtree: true },
  };
  
function useMutationObservable(targetEl, cb, options = DEFAULT_OPTIONS) {
    const [observer, setObserver] = useState(null);
  
    useEffect(() => {
      const obs = new MutationObserver(cb);
      setObserver(obs);
    }, [cb, options, setObserver]);
  
    useEffect(() => {
      if (!observer) return;
      const { config } = options;
      observer.observe(targetEl, config);
      return () => {
        if (observer) {
          observer.disconnect();
        }
      };
    }, [observer, targetEl, options]);
}