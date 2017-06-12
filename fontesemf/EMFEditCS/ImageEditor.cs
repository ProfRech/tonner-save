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
        public int Edit(string input, string output, int percentage)
        {
            //Bitmap meta = new Bitmap(input);
            //Bitmap bit = (Bitmap)meta.Clone();
            //meta.Dispose();
            Bitmap bit = new Bitmap(input);

            Graphics g = Graphics.FromImage(bit);
            SolidBrush myBrush = new SolidBrush(Color.FromArgb((0xff * percentage) / 100, 0xff, 0xff, 0xff));
            g.FillRectangle(myBrush, new Rectangle(0, 0, bit.Width, bit.Height));
            bit.Save(output, System.Drawing.Imaging.ImageFormat.Emf);
            g.Dispose();

            /*
            for (int x = 0, flag = 0; x < bit.Width; x++, flag = flag == 0 ? 1 : 0)
            {
                for (int y = flag; y < bit.Height; y += 2)
                {
                    Color newC = Color.FromArgb(0);
                    bit.SetPixel(x, y, newC);
                }
            }
            bit.Save(output, System.Drawing.Imaging.ImageFormat.Emf);
            */

            /*
            for (int x = 0; x < bit.Width; x++)
            {
                for (int y = 0; y < bit.Height; y++)
                {
                    Color currentC = bit.GetPixel(x, y);
                    if ((currentC.ToArgb() & 0x00FFFFFFu) == 0x00FFFFFFu) continue;// se for branco
                    if ((currentC.ToArgb() | 0x00FFFFFFu) == 0x00FFFFFFu) continue;// se for transparente
                    Color newC = Color.FromArgb(currentC.A,
                        (currentC.R * (100 - percentage) + 0xFF * percentage) / 100,
                        (currentC.G * (100 - percentage) + 0xFF * percentage) / 100,
                        (currentC.B * (100 - percentage) + 0xFF * percentage) / 100);
                    bit.SetPixel(x, y, newC);
                }
            }
            bit.Save(output, System.Drawing.Imaging.ImageFormat.Emf);
            */

            bit.Dispose();

            return 0;
        }
    }
}
