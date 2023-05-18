import {BsChevronCompactLeft, BsChevronCompactRight, BsCaretLeftFill, BsCaretRightFill} from 'react-icons/bs'

export default function ScrollButton({direction, onClick}){

    return (
    (direction == "left") ? 
    (<BsChevronCompactLeft id="left-scroll" className='scroll-btns' onClick={onClick}/>)
    : 
    (<BsChevronCompactRight id='right-scroll' className='scroll-btns' onClick={onClick}/>)
    )
}