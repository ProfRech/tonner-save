using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;

namespace EMFEditCS
{
    [ComVisible(true)]
    public interface IImageEditor
    {
        int Edit(string input, string output, int percentage);
    }

    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.None)]
    public class ImageEditor : IImageEditor
    {
        #pragma warning disable CS0168
        public int Edit(string input, string output, int percentage)
        {
            Bitmap bit = null;
            Graphics g = null;
            try
            {
                try
                {
                    bit = new Bitmap(input);
                    g = Graphics.FromImage(bit);
                }
                catch (ArgumentException e) { return 1; } // Error: invalid input file

                try
                {
                    SolidBrush myBrush = new SolidBrush(Color.FromArgb((0xff * percentage) / 100, 0xff, 0xff, 0xff));
                    g.FillRectangle(myBrush, new Rectangle(0, 0, bit.Width, bit.Height));
                }catch (ArgumentException e) { return 2; } // Error: the percentage value must be between 0 and 100


                try
                {
                    bit.Save(output, System.Drawing.Imaging.ImageFormat.Emf);
                }
                catch (ArgumentException e) { return 3; } // Error: invalid output file
                catch (ExternalException e) { return 4; } // Error: the input file must be different than the output file

                return 0; // Successful
            }
            catch (Exception e) { return 5; } // Generic error
            finally
            {
                if(g != null)
                    g.Dispose();
                if(bit != null)
                    bit.Dispose();
            }
        }
    }
}
