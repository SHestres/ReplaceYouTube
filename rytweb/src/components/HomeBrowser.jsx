import Layout from "./Layout"
import GenreCollection from "./GenreCollection"

export default function HomeBrowser(){


    return (
        <Layout>
            <GenreCollection key="mov-favs" genre={{name: "movies"}} category="favorites" title="Movie Favorites"/>
            <GenreCollection key="yt-favs" genre={{name: "youtube"}} category="favorites" title="Youtube Favorites"/>
            <GenreCollection key="other-favs" genre={{name: "other"}} category="favorites" title="Other Favorites"/>
        </Layout>
    )
}