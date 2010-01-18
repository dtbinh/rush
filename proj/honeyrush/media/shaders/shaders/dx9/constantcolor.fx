Technique FFP
{
    Pass P0 
    {        
        colorop[0]          = SelectArg1;
        colorarg1[0]        = TFactor;
        
        alphaop[0]          = Modulate;
        alphaarg1[0]        = Texture;
        alphaarg2[0]        = TFactor;
    }
};