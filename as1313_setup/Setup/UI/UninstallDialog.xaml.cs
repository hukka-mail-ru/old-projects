﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using Setup.Common;

namespace Setup.UI
{
    /// <summary>
    /// Interaction logic for UninstallDialog.xaml
    /// </summary>
    public partial class UninstallDialog : Window
    {
        public UninstallDialog()
        {
            InitializeComponent();
        }

        private void UninstallButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                General.Rollback();
                UI.ShowDialog(this, new FinishDialog());
            }
            catch (Exception ex)
            {
                Message.Show(ex);
            }

        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                UI.CloseDialog(this);
            }
            catch (Exception ex)
            {
                Message.Show(ex);
            }
        }
    }
}
