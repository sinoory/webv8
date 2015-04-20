

package Test;

sub parseExtendedAttributes
{
    print "===========\n";
    my $str = shift;
    $str =~ s/\[\s*(.*?)\s*\]/$1/g;
    print "str=$str\n";

    my %attrs = ();

    foreach my $value (split(/\s*,\s*/, $str)) {
        print "varlue=$value\n";
        (my $name, my $val) = split(/\s*=\s*/, $value, 2);
        print "name=$name,val=$val\n";

        # Attributes with no value are set to be true
        $val = 1 unless defined $val;
        $attrs{$name} = $val;
        die("Invalid extended attribute name: '$name'\n") if $name =~ /\s/;
    }

    return \%attrs;
}


my $at="";
$at="[NotEnumerable, Conditional=WILL_REVEAL_EDGE_EVENTS";
parseExtendedAttributes($at);

$at="[Conditional=POINTER_LOCK";
parseExtendedAttributes($at);


$at="Conditional=POINTER_LOCK";
parseExtendedAttributes($at);
