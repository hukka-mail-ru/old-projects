﻿// -----------------------------------------------------------------------
// <copyright file="General.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace Setup.UI
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class General
    {
        public static bool SureToExit()
        {
            return MessageBox.Show(
                "Are you sure you want to cancel the AS1313 Client Installation?", 
                "QUESTION", MessageBoxButtons.YesNo) == DialogResult.Yes;
        }
    }
}