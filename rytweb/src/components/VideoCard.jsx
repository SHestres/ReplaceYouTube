

export default function VideoCard({Title, id, vote_average, poster, Images }) {

    let style = {
        backgroundImage: `url(${Images[0]})`,//`url(/images${imgSrc})`;
        backgroundSize: 'contain',
        backgroundRepeat: 'no-repeat',
        width: '100%',

    }

    return <div className="video-card" style={style}>
        <div className="video-details">
            <h4>{Title}</h4>
        </div>
    </div>
}