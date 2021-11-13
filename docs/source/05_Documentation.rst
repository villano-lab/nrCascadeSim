.. somewhere in here put notes on how to build and render a static page with and install sphinx

=================================
5. Documentation and Sphinx 
=================================

This documentation is hosted via `readthedocs <https://nrcascadesim.readthedocs.io/en/latest/>`_
but can also be built using `sphinx
<https://www.sphinx-doc.org/en/master/tutorial/getting-started.html>`_ in a stand-alone way. 

To build the documentation do the following starting at the root directory of the `repository
<https://github.com/villano-lab/nrCascadeSim>`_. 

.. code-block:: bash

   pip install sphinx
   sphinx-build --version
   sphinx-build -b html docs/source/ docs/build/html

This will build html documentation in `docs/build/html/index.html` which you can view with your
browser. 

After making the documentation there will be a `Makefile` inside the `docs/` directory that can be
used for further building. You can use the following commands, then, to re-build the `html`
documentation, or build the `epub` documentation, or `latex` and `pdf` documentation.

.. code-block:: bash

   cd docs/
   make html
   make epub
   make latexpdf

.. **protoSENSEI@surface**
.. ^^^^^^^^^^^^^^^^^^^^^^^
.. 
.. * **SENSEI: First Direct-Detection Constraints on sub-GeV Dark Matter from a Surface Run**
..   
..   SENSEI Collaboration (Michael Crisler et al.)
.. 
..   .. image:: https://img.shields.io/badge/Phys.Rev.Lett.-121(2018)no.6-255773.svg
..       :target: https://journals.aps.org/prl/abstract/10.1103/PhysRevLett.121.061803
..       :alt: Phys.Rev.Lett. 121 (2018) no.6, 061803
..   .. image:: https://img.shields.io/badge/arXiv-1804.00088-B31B1B.svg
..       :target: https://arxiv.org/abs/1804.00088
..       :alt: [arXiv:1804.00088]


