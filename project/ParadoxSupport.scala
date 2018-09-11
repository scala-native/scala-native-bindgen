package org.scalanative.bindgen.build

import com.lightbend.paradox.markdown._
import com.lightbend.paradox.sbt.ParadoxPlugin.autoImport.paradoxDirectives
import org.pegdown.Printer
import org.pegdown.ast.{DirectiveNode, TextNode, Visitor}
import scala.collection.JavaConverters._

object ParadoxSupport {
  val paradoxWithCustomDirectives = Seq(
    paradoxDirectives ++= Seq(
      { context: Writer.Context ⇒
        new BindingDependencyDirective(context.location.tree.label,
                                       context.properties)
      }
    )
  )

  /* Based on the DependencyDirective from Paradox. */
  case class BindingDependencyDirective(page: Page,
                                        variables: Map[String, String])
      extends LeafBlockDirective("binding") {
    val projectVersion     = variables("project.version")
    val scalaBinaryVersion = variables("scala.binary.version")

    def render(node: DirectiveNode,
               visitor: Visitor,
               printer: Printer): Unit = {
      node.contentsNode.getChildren.asScala.headOption match {
        case Some(text: TextNode) =>
          renderBindingDependency(text.getText, printer)
        case _ => node.contentsNode.accept(visitor)
      }
    }

    def renderBindingDependency(binding: String, printer: Printer): Unit = {
      val group        = "org.scala-native.bindings"
      val artifactName = binding
      val artifactId   = s"${artifactName}_native0.3_${scalaBinaryVersion}"
      val bintrayRepo  = "http://dl.bintray.com/scala-native-bindgen/maven"

      printer.print(
        s"""
           |<dl>
           |<dt>sbt</dt>
           |<dd>
           |<pre class="prettyprint"><code class="language-scala">resolvers += Resolver.bintrayRepo("scala-native-bindgen", "maven")
           |libraryDependencies += "${group}" %%% "${artifactName}" % "${projectVersion}"
           |</code></pre>
           |</dd>
           |
           |<dt>Maven</dt>
           |<dd>
           |<pre class="prettyprint"><code class="language-xml">&lt;repositories&gt;
           |  &lt;repository&gt;
           |    &lt;id&gt;maven&lt;/id&gt;
           |    &lt;url&gt;${bintrayRepo}&lt;/url&gt;
           |  &lt;/repository&gt;
           |&lt;/repositories&gt;
           |
           |&lt;dependencies&gt;
           |  &lt;dependency&gt;
           |    &lt;groupId&gt;${group}&lt;/groupId&gt;
           |    &lt;artifactId&gt;${artifactId}&lt;/artifactId&gt;
           |    &lt;version&gt;${projectVersion}&lt;/version&gt;
           |  &lt;/dependency&gt;
           |&lt;/dependencies&gt;
           |</code></pre>
           |</dd>
           |
           |<dt>Gradle</dt>
           |<dd>
           |<pre class="prettyprint"><code class="language-gradle">repositories {
           |  maven {
           |    url "${bintrayRepo}"
           |  }
           |}
           |
           |dependencies {
           |  compile group: '${group}', name: '${artifactId}', version: '${projectVersion}'
           |}
           |</code></pre>
           |</dd>
           |</dl>
           |""".stripMargin
      )
    }
  }
}
