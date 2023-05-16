import {BsCaretLeftFill, BsCaretRightFill} from 'react-icons/bs'

export default function ScrollButton(onClick, direction){

    return direction === 'left' ? 
    <BsCaretLeftFill id="left-scroll" className='scroll-btns' onClick={onClick}/> 
    : 
    <BsCaretRightFill id='right-scroll' className='scroll-btns' onClick={onClick}/>

}