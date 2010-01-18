Technique FFP
{
    Pass P0 
    {
        colorop[0]             = selectarg1;
        colorarg1[0]           = diffuse;
        alphaop[0]             = selectarg1;
        alphaarg1[0]           = diffuse;
    }
};